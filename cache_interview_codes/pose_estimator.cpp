#include <Eigen/Dense>
#include <Eigen/Geometry>

/*
g++ -std=c++17 pose_estimator.cpp \
    -I/usr/include/eigen3 \
    -O2 \
    -o pose_estimator

./pose_estimator
*/
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace Eigen;
using namespace std;

// ============================================================
// 15-state Error-State EKF Pose Estimator
//
// Nominal state:
//   p_W   : position in world frame
//   v_W   : velocity in world frame
//   R_WE  : rotation Ego -> World
//   ba    : accelerometer bias
//   bg    : gyroscope bias
//
// Error state:
//   dx = [dp, dv, dtheta, dba, dbg]^T
//
// IMU:
//   gyro = omega + bg + ng
//   accel = specific_force + ba + na
//
// Other measurements:
//   GNSS position
//   wheel longitudinal velocity
//
// Coordinate convention:
//   World Z is UP
//   gravity = [0, 0, -9.81]
// ============================================================

constexpr int STATE_SIZE = 15;
using Matrix15d = Matrix<double, STATE_SIZE, STATE_SIZE>;
using Vector15d = Matrix<double, STATE_SIZE, 1>;
// ============================================================
// Utility: skew symmetric matrix
// skew(a) * b = a x b
// ============================================================
Matrix3d skew(const Vector3d& v)
{
    Matrix3d S;
    S <<
         0.0,   -v.z(),  v.y(),
         v.z(),  0.0,   -v.x(),
        -v.y(),  v.x(),  0.0;

    return S;
}
// ============================================================
// SO(3) exponential map
//
// rotation vector phi -> rotation matrix
// ============================================================

Matrix3d expSO3(const Vector3d& phi)
{
    double theta = phi.norm();
    Matrix3d A = skew(phi);
    if (theta < 1e-8)
    {
        // First/second order approximation
        return Matrix3d::Identity()
             + A
             + 0.5 * A * A;
    }

    double a = sin(theta) / theta;
    double b =
        (1.0 - cos(theta))
        / (theta * theta);

    return Matrix3d::Identity()
         + a * A
         + b * A * A;
}
// ============================================================
// SO(3) logarithm
// ============================================================

Vector3d logSO3(const Matrix3d& R)
{
    double cosTheta =
        (R.trace() - 1.0) * 0.5;
    cosTheta =
        std::clamp(cosTheta, -1.0, 1.0);
    double theta =
        acos(cosTheta);

    if (theta < 1e-8)
    {
        return Vector3d(
            (R(2,1) - R(1,2)) * 0.5,
            (R(0,2) - R(2,0)) * 0.5,
            (R(1,0) - R(0,1)) * 0.5);
    }

    Vector3d w;
    w <<
        R(2,1) - R(1,2),
        R(0,2) - R(2,0),
        R(1,0) - R(0,1);

    return
        theta
        / (2.0 * sin(theta))
        * w;
}

// ============================================================
// Re-orthogonalize rotation matrix
// Numerical integration can slowly violate R^T R = I.
// ============================================================
Matrix3d normalizeRotation(const Matrix3d& R)
{
    JacobiSVD<Matrix3d> svd(
        R,
        ComputeFullU | ComputeFullV);

    Matrix3d U = svd.matrixU();
    Matrix3d V = svd.matrixV();

    Matrix3d result =
        U * V.transpose();

    if (result.determinant() < 0)
    {
        U.col(2) *= -1.0;

        result =
            U * V.transpose();
    }

    return result;
}

// ============================================================
// Homogeneous SE(3) pose
// ============================================================

Matrix4d makePose(
    const Matrix3d& R_WE,
    const Vector3d& p_W)
{
    Matrix4d T =
        Matrix4d::Identity();
    T.block<3,3>(0,0) =
        R_WE;
    T.block<3,1>(0,3) =
        p_W;
    return T;
}


// ============================================================
// Sensor data structures
// ============================================================

struct ImuMeasurement
{
    double timestamp;

    Vector3d accel; // m/s^2 specific force
    Vector3d gyro;  // rad/s
};

struct GnssMeasurement
{
    double timestamp;
    Vector3d position;
    Matrix3d covariance;
};

struct WheelMeasurement
{
    double timestamp;
    double longitudinalVelocity;
    double variance;
};

// ============================================================
// Pose sample for interpolation / LiDAR deskewing
// ============================================================

struct PoseSample
{
    double timestamp;
    Vector3d position;
    Quaterniond orientation;
};


// ============================================================
// Nominal navigation state
// ============================================================

struct NavigationState
{
    double timestamp = 0.0;
    Vector3d position =
        Vector3d::Zero();

    Vector3d velocity =
        Vector3d::Zero();

    Matrix3d R_WE =
        Matrix3d::Identity();

    Vector3d accelBias =
        Vector3d::Zero();

    Vector3d gyroBias =
        Vector3d::Zero();
};

// ============================================================
// Error State EKF
// ============================================================

class ErrorStateEKF
{
public:

    ErrorStateEKF()
    {
        P_.setZero();
        // Initial position uncertainty
        P_.block<3,3>(0,0) =
            Matrix3d::Identity() * 4.0;

        // Initial velocity uncertainty
        P_.block<3,3>(3,3) =
            Matrix3d::Identity() * 1.0;

        // Initial attitude uncertainty
        // approximately 5 degrees
        double attitudeSigma =
            5.0 * M_PI / 180.0;

        P_.block<3,3>(6,6) =
            Matrix3d::Identity()
            * attitudeSigma
            * attitudeSigma;

        // Accelerometer bias uncertainty
        P_.block<3,3>(9,9) =
            Matrix3d::Identity() * 0.01;

        // Gyro bias uncertainty
        P_.block<3,3>(12,12) =
            Matrix3d::Identity() * 0.001;
    }

    // ========================================================
    // Noise configuration
    // ========================================================

    void setNoiseParameters(
        double accelNoise,
        double gyroNoise,
        double accelBiasNoise,
        double gyroBiasNoise)
    {
        sigmaAccel_ =
            accelNoise;

        sigmaGyro_ =
            gyroNoise;

        sigmaAccelBias_ =
            accelBiasNoise;

        sigmaGyroBias_ =
            gyroBiasNoise;
    }


    // ========================================================
    // Initialization
    // ========================================================
    void initialize(
        double timestamp,
        const Vector3d& position,
        const Vector3d& velocity,
        const Matrix3d& R_WE,
        const Vector3d& accelBias =
            Vector3d::Zero(),
        const Vector3d& gyroBias =
            Vector3d::Zero())
    {
        state_.timestamp =
            timestamp;

        state_.position =
            position;

        state_.velocity =
            velocity;

        state_.R_WE =
            normalizeRotation(R_WE);

        state_.accelBias =
            accelBias;

        state_.gyroBias =
            gyroBias;

        initialized_ = true;
        savePose();
    }

    // ========================================================
    // IMU propagation
    // ========================================================
    void processImu(
        const ImuMeasurement& imu)
    {
        if (!initialized_)
            return;

        double dt =
            imu.timestamp
            - state_.timestamp;

        if (dt <= 0.0)
            return;

        // Avoid huge propagation if sensor stream has a gap.
        if (dt > 0.1)
        {
            cerr
                << "Warning: large IMU dt = "
                << dt
                << " seconds\n";
        }

        // ----------------------------------------------------
        // Bias corrected IMU
        // ----------------------------------------------------

        Vector3d omega =
            imu.gyro
            - state_.gyroBias;

        Vector3d specificForce =
            imu.accel
            - state_.accelBias;


        // ----------------------------------------------------
        // Store old rotation because acceleration measurement
        // corresponds approximately to beginning of interval.
        // ----------------------------------------------------
        Matrix3d R_old =
            state_.R_WE;

        // ----------------------------------------------------
        // World acceleration
        //
        // a_W = R_WE * f_E + g_W
        // ----------------------------------------------------
        Vector3d accelerationWorld =
            R_old * specificForce
            + gravity_;

        // ----------------------------------------------------
        // Position integration
        // ----------------------------------------------------
        state_.position +=
            state_.velocity * dt
            + 0.5
            * accelerationWorld
            * dt * dt;
        // ----------------------------------------------------
        // Velocity integration
        // ----------------------------------------------------

        state_.velocity +=
            accelerationWorld * dt;
        // ----------------------------------------------------
        // Orientation integration
        //
        // R_{k+1}
        // =
        // R_k Exp((omega-bg) dt)
        // ----------------------------------------------------

        state_.R_WE =
            R_old
            * expSO3(
                omega * dt);

        state_.R_WE =
            normalizeRotation(
                state_.R_WE);

        // ----------------------------------------------------
        // Covariance propagation
        // ----------------------------------------------------
        propagateCovariance(
            specificForce,
            omega,
            R_old,
            dt);

        state_.timestamp =
            imu.timestamp;

        savePose();
    }


    // ========================================================
    // GNSS position update
    // z = p_W + noise
    // ========================================================

    bool processGnss(
        const GnssMeasurement& gnss)
    {
        if (!initialized_)
            return false;

        // Measurement should normally arrive after propagation
        // to approximately the same timestamp.

        Vector3d predicted =
            state_.position;

        Vector3d residual =
            gnss.position
            - predicted;

        Matrix<double,3,15> H;
        H.setZero();
        H.block<3,3>(0,0) =
            Matrix3d::Identity();

        Matrix3d S =
            H * P_ * H.transpose()
            + gnss.covariance;

        double mahalanobis =
            residual.transpose()
            * S.inverse()
            * residual;

        // 99% chi-square threshold for 3 DOF
        constexpr double gate =
            11.345;

        cout
            << "GNSS Mahalanobis d^2 = "
            << mahalanobis
            << "\n";

        if (mahalanobis > gate)
        {
            cout
                << "GNSS rejected by gate\n";

            return false;
        }

        Matrix<double,15,3> K =
            P_
            * H.transpose()
            * S.inverse();

        Vector15d dx =
            K * residual;

        injectError(dx);
        // Joseph covariance update
        Matrix15d I =
            Matrix15d::Identity();

        Matrix15d A =
            I - K * H;

        P_ =
            A * P_ * A.transpose()
            + K
            * gnss.covariance
            * K.transpose();

        symmetrizeCovariance();
        savePose();
        return true;
    }
    // ========================================================
    // Wheel speed update
    // Wheel measures forward body velocity:
    // z = e_x^T R_EW v_W
    // ========================================================
    bool processWheel(
        const WheelMeasurement& wheel)
    {
        if (!initialized_)
            return false;

        Matrix3d R_EW =
            state_.R_WE.transpose();

        Vector3d velocityBody =
            R_EW
            * state_.velocity;

        double predicted =
            velocityBody.x();

        double residual =
            wheel.longitudinalVelocity
            - predicted;

        Matrix<double,1,15> H;
        H.setZero();

        // derivative wrt world velocity
        H.block<1,3>(0,3) =
            R_EW.row(0);
        // ----------------------------------------------------
        // Approximate attitude Jacobian.
        // With right multiplicative attitude error:
        // R_true = R_hat Exp(dtheta)
        // R_true^T v
        // ~
        // (I - [dtheta]x) R_hat^T v
        //
        // delta(v_body)
        // =
        // [v_body]x dtheta
        // ----------------------------------------------------

        Matrix3d Jtheta =
            skew(velocityBody);

        H.block<1,3>(0,6) =
            Jtheta.row(0);

        double S =
            (H * P_ * H.transpose())(0,0)
            + wheel.variance;

        double d2 =
            residual * residual / S;

        // 99% chi-square threshold, 1 DOF
        constexpr double gate =
            6.635;


        if (d2 > gate)
        {
            cout
                << "Wheel measurement rejected. d^2="
                << d2
                << "\n";

            return false;
        }

        Matrix<double,15,1> K =
            P_
            * H.transpose()
            / S;

        Vector15d dx =
            K * residual;

        injectError(dx);

        Matrix15d I =
            Matrix15d::Identity();

        Matrix15d A =
            I - K * H;

        Matrix<double,1,1> R;

        R(0,0) =
            wheel.variance;

        P_ =
            A * P_ * A.transpose()
            + K * R * K.transpose();

        symmetrizeCovariance();
        return true;
    }

    // ========================================================
    // Non-holonomic constraint
    // Vehicle lateral/vertical body velocity ~= 0.
    // z = [0,0]
    // h(x) = [v_body_y, v_body_z]
    // ========================================================

    bool applyNonHolonomicConstraint(
        double variance = 0.05)
    {
        Matrix3d R_EW =
            state_.R_WE.transpose();

        Vector3d vBody =
            R_EW * state_.velocity;

        Vector2d residual;

        residual <<
            -vBody.y(),
            -vBody.z();

        Matrix<double,2,15> H;
        H.setZero();
        // Velocity Jacobian
        H.block<1,3>(0,3) =
            R_EW.row(1);

        H.block<1,3>(1,3) =
            R_EW.row(2);

        // Attitude Jacobian
        Matrix3d Jtheta =
            skew(vBody);

        H.block<1,3>(0,6) =
            Jtheta.row(1);

        H.block<1,3>(1,6) =
            Jtheta.row(2);

        Matrix2d R =
            Matrix2d::Identity()
            * variance;

        Matrix2d S =
            H * P_ * H.transpose()
            + R;

        double d2 =
            residual.transpose()
            * S.inverse()
            * residual;


        // 99% for 2 DOF
        if (d2 > 9.210)
            return false;

        Matrix<double,15,2> K =
            P_
            * H.transpose()
            * S.inverse();

        Vector15d dx =
            K * residual;

        injectError(dx);

        Matrix15d I =
            Matrix15d::Identity();

        Matrix15d A =
            I - K * H;

        P_ =
            A * P_ * A.transpose()
            + K * R * K.transpose();

        symmetrizeCovariance();

        return true;
    }


    // ========================================================
    // Accessors
    // ========================================================

    const NavigationState&
    state() const
    {
        return state_;
    }


    const Matrix15d&
    covariance() const
    {
        return P_;
    }

    Matrix4d pose() const
    {
        return makePose(
            state_.R_WE,
            state_.position);
    }

    const vector<PoseSample>&
    poseHistory() const
    {
        return poseHistory_;
    }

    // ========================================================
    // Pose interpolation
    // Useful for LiDAR deskewing.
    // ========================================================
    bool interpolatePose(
        double timestamp,
        Matrix4d& T_WE) const
    {
        if (poseHistory_.empty())
            return false;

        if (timestamp <
                poseHistory_.front().timestamp ||
            timestamp >
                poseHistory_.back().timestamp)
        {
            return false;
        }

        auto upper =
            lower_bound(
                poseHistory_.begin(),
                poseHistory_.end(),
                timestamp,
                [](const PoseSample& sample,
                   double t)
                {
                    return sample.timestamp < t;
                });


        if (upper == poseHistory_.begin())
        {
            const auto& s =
                *upper;

            T_WE =
                makePose(
                    s.orientation.toRotationMatrix(),
                    s.position);

            return true;
        }

        if (upper == poseHistory_.end())
        {
            const auto& s =
                poseHistory_.back();

            T_WE =
                makePose(
                    s.orientation.toRotationMatrix(),
                    s.position);

            return true;
        }

        const PoseSample& b =
            *upper;

        const PoseSample& a =
            *(upper - 1);

        double alpha =
            (timestamp - a.timestamp)
            /
            (b.timestamp - a.timestamp);

        Vector3d position =
            (1.0 - alpha) * a.position
            + alpha * b.position;

        Quaterniond orientation =
            a.orientation.slerp(
                alpha,
                b.orientation);

        orientation.normalize();

        T_WE =
            makePose(
                orientation.toRotationMatrix(),
                position);

        return true;
    }

private:
    NavigationState state_;

    Matrix15d P_ =
        Matrix15d::Identity();

    bool initialized_ =
        false;

    Vector3d gravity_ =
        Vector3d(
            0.0,
            0.0,
            -9.81);

    // Typical illustrative values.
    // These MUST be replaced with values from
    // actual IMU noise characterization/datasheet.
    double sigmaAccel_ =
        0.10;

    double sigmaGyro_ =
        0.01;

    double sigmaAccelBias_ =
        0.001;

    double sigmaGyroBias_ =
        0.0001;

    vector<PoseSample> poseHistory_;
    // ========================================================
    // Covariance propagation
    //
    // dx_dot = F dx + G n
    // ========================================================
    void propagateCovariance(
        const Vector3d& specificForce,
        const Vector3d& omega,
        const Matrix3d& R_WE,
        double dt)
    {
        Matrix15d F =
            Matrix15d::Zero();
        // dp_dot = dv
        F.block<3,3>(0,3) =
            Matrix3d::Identity();
        // dv_dot =
        // -R [a]x dtheta
        // -R dba
        // -R na
        F.block<3,3>(3,6) =
            -R_WE
            * skew(specificForce);

        F.block<3,3>(3,9) =
            -R_WE;

        // dtheta_dot =
        // -[omega]x dtheta
        // -dbg
        // -ng
        F.block<3,3>(6,6) =
            -skew(omega);

        F.block<3,3>(6,12) =
            -Matrix3d::Identity();
        // ----------------------------------------------------
        // Noise vector:
        //
        // n =
        // [na, ng, nba, nbg]
        //
        // dimension 12
        // ----------------------------------------------------

        Matrix<double,15,12> G;
        G.setZero();
        // accel noise
        G.block<3,3>(3,0) =
            -R_WE;
        // gyro noise
        G.block<3,3>(6,3) =
            -Matrix3d::Identity();

        // accel bias random walk
        G.block<3,3>(9,6) =
            Matrix3d::Identity();

        // gyro bias random walk
        G.block<3,3>(12,9) =
            Matrix3d::Identity();
        Matrix<double,12,12> Qc;
        Qc.setZero();

        Qc.block<3,3>(0,0) =
            Matrix3d::Identity()
            * sigmaAccel_
            * sigmaAccel_;

        Qc.block<3,3>(3,3) =
            Matrix3d::Identity()
            * sigmaGyro_
            * sigmaGyro_;

        Qc.block<3,3>(6,6) =
            Matrix3d::Identity()
            * sigmaAccelBias_
            * sigmaAccelBias_;

        Qc.block<3,3>(9,9) =
            Matrix3d::Identity()
            * sigmaGyroBias_
            * sigmaGyroBias_;
        // ----------------------------------------------------
        // First-order discretization:
        // Phi ~= I + F dt
        // Qd ~= G Qc G^T dt
        // For production:
        // Van Loan or higher-order discretization is better.
        // ----------------------------------------------------

        Matrix15d Phi =
            Matrix15d::Identity()
            + F * dt;

        Matrix15d Qd =
            G * Qc * G.transpose()
            * dt;

        P_ =
            Phi
            * P_
            * Phi.transpose()
            + Qd;

        symmetrizeCovariance();
    }

    // ========================================================
    // Inject error-state correction into nominal state
    // ========================================================
    void injectError(
        const Vector15d& dx)
    {
        Vector3d dp =
            dx.segment<3>(0);

        Vector3d dv =
            dx.segment<3>(3);

        Vector3d dtheta =
            dx.segment<3>(6);

        Vector3d dba =
            dx.segment<3>(9);

        Vector3d dbg =
            dx.segment<3>(12);

        state_.position +=
            dp;

        state_.velocity +=
            dv;

        state_.R_WE =
            state_.R_WE
            * expSO3(dtheta);

        state_.R_WE =
            normalizeRotation(
                state_.R_WE);

        state_.accelBias +=
            dba;

        state_.gyroBias +=
            dbg;

        // ----------------------------------------------------
        // Error-state reset Jacobian
        //
        // After injecting dtheta into nominal orientation,
        // reset attitude error around zero.
        //
        // First-order approximation.
        // ----------------------------------------------------
        Matrix15d Greset =
            Matrix15d::Identity();

        Greset.block<3,3>(6,6) =
            Matrix3d::Identity()
            - 0.5 * skew(dtheta);

        P_ =
            Greset
            * P_
            * Greset.transpose();

        symmetrizeCovariance();
    }
    void symmetrizeCovariance()
    {
        P_ =
            0.5
            * (P_ + P_.transpose());
    }

    void savePose()
    {
        PoseSample sample;
        sample.timestamp =
            state_.timestamp;

        sample.position =
            state_.position;

        sample.orientation =
            Quaterniond(
                state_.R_WE);

        sample.orientation.normalize();
        // Prevent duplicate timestamps
        if (!poseHistory_.empty() &&
            fabs(
                poseHistory_.back().timestamp
                - sample.timestamp)
                < 1e-9)
        {
            poseHistory_.back() =
                sample;
            return;
        }

        poseHistory_.push_back(
            sample);

        // Keep bounded history.
        //
        // Enough for short-term LiDAR deskewing.
        constexpr size_t MAX_HISTORY =
            10000;

        if (poseHistory_.size()
            > MAX_HISTORY)
        {
            poseHistory_.erase(
                poseHistory_.begin(),
                poseHistory_.begin()
                + 1000);
        }
    }
};

// ============================================================
// Euler angles only for display
// ============================================================

Vector3d rotationToEulerRPY(
    const Matrix3d& R)
{
    // ZYX convention:
    //
    // R = Rz(yaw) Ry(pitch) Rx(roll)

    double pitch =
        asin(
            std::clamp(
                -R(2,0),
                -1.0,
                1.0));
    double roll =
        atan2(
            R(2,1),
            R(2,2));

    double yaw =
        atan2(
            R(1,0),
            R(0,0));

    return Vector3d(
        roll,
        pitch,
        yaw);
}
// ============================================================
// Print state
// ============================================================
void printState(
    const ErrorStateEKF& estimator,
    const string& label)
{
    const auto& x =
        estimator.state();

    Vector3d rpy =
        rotationToEulerRPY(
            x.R_WE);

    constexpr double RAD_TO_DEG =
        180.0 / M_PI;

    cout
        << "\n========================================\n"
        << label
        << "\n"
        << "========================================\n";
    cout
        << "time     : "
        << x.timestamp
        << "\n";
    cout
        << "position : "
        << x.position.transpose()
        << " m\n";
    cout
        << "velocity : "
        << x.velocity.transpose()
        << " m/s\n";

    cout
        << "RPY      : "
        << (rpy * RAD_TO_DEG).transpose()
        << " deg\n";

    cout
        << "acc bias : "
        << x.accelBias.transpose()
        << "\n";

    cout
        << "gyro bias: "
        << x.gyroBias.transpose()
        << "\n";


    cout
        << "position stddev: "
        << sqrt(estimator.covariance()(0,0))
        << " "
        << sqrt(estimator.covariance()(1,1))
        << " "
        << sqrt(estimator.covariance()(2,2))
        << " m\n";


    cout
        << "\nT_WE:\n"
        << estimator.pose()
        << "\n";
}


// ============================================================
// Demonstration
//
// Vehicle:
//   initial velocity = 10 m/s forward
//   straight road
//
// IMU:
//   stationary specific force relative to vehicle acceleration
//   is +9.81 m/s^2 in body Z because accelerometer measures
//   specific force opposing gravity.
//
// Therefore:
//
//   R * [0,0,9.81] + [0,0,-9.81] = 0
//
// so constant velocity.
// ============================================================
int main(int argc, char** argv)
{
    cout
        << fixed
        << setprecision(6);

    ErrorStateEKF estimator;
    estimator.setNoiseParameters(
        0.10,     // accelerometer noise
        0.01,     // gyro noise
        0.001,    // accel bias random walk
        0.0001);  // gyro bias random walk


    // ========================================================
    // Initialize
    // ========================================================

    estimator.initialize(
        0.0,
        Vector3d(
            0.0,
            0.0,
            0.0),
        Vector3d(
            10.0,
            0.0,
            0.0),
        Matrix3d::Identity());


    printState(
        estimator,
        "INITIAL STATE");


    // ========================================================
    // Simulate 5 seconds
    //
    // IMU  : 200 Hz
    // GNSS : 10 Hz
    // Wheel: 50 Hz
    // ========================================================

    const double imuDt =
        0.005;

    const int steps =
        static_cast<int>(
            5.0 / imuDt);


    for (int k = 1;
         k <= steps;
         ++k)
    {
        double t =
            k * imuDt;


        // ----------------------------------------------------
        // Simulated IMU
        //
        // Vehicle travels at constant velocity.
        //
        // Specific force:
        //
        // f = R^T(a - g)
        //
        // a = 0
        // g = [0,0,-9.81]
        //
        // therefore:
        //
        // f = [0,0,9.81]
        // ----------------------------------------------------

        ImuMeasurement imu;

        imu.timestamp =
            t;

        imu.accel =
            Vector3d(
                0.0,
                0.0,
                9.81);

        imu.gyro =
            Vector3d::Zero();


        estimator.processImu(
            imu);


        // ----------------------------------------------------
        // Wheel update at 50 Hz
        // ----------------------------------------------------

        if (k % 4 == 0)
        {
            WheelMeasurement wheel;

            wheel.timestamp =
                t;

            wheel.longitudinalVelocity =
                10.0;

            wheel.variance =
                0.04; // sigma = 0.2 m/s


            estimator.processWheel(
                wheel);


            estimator.applyNonHolonomicConstraint(
                0.02);
        }


        // ----------------------------------------------------
        // GNSS update at 10 Hz
        //
        // True position:
        //
        // x = 10*t
        // ----------------------------------------------------

        if (k % 20 == 0)
        {
            GnssMeasurement gnss;

            gnss.timestamp =
                t;


            // Add deterministic small "noise"
            // so demonstration is repeatable.

            double noiseX =
                0.10 * sin(2.0 * t);

            double noiseY =
                0.08 * cos(1.3 * t);

            double noiseZ =
                0.05 * sin(0.7 * t);


            gnss.position =
                Vector3d(
                    10.0 * t
                        + noiseX,
                    noiseY,
                    noiseZ);


            gnss.covariance =
                Matrix3d::Zero();

            gnss.covariance(0,0) =
                0.25;

            gnss.covariance(1,1) =
                0.25;

            gnss.covariance(2,2) =
                0.50;


            estimator.processGnss(
                gnss);
        }


        // Print every second

        if (k % 200 == 0)
        {
            printState(
                estimator,
                "STATE @ "
                + to_string(t)
                + " sec");
        }
    }


    // ========================================================
    // Pose interpolation example
    //
    // This is exactly what LiDAR deskewing needs.
    // ========================================================

    double lidarPointTimestamp =
        4.973;


    Matrix4d T_WE_point;


    if (estimator.interpolatePose(
            lidarPointTimestamp,
            T_WE_point))
    {
        cout
            << "\n========================================\n"
            << "INTERPOLATED POSE FOR LIDAR POINT\n"
            << "========================================\n";


        cout
            << "timestamp = "
            << lidarPointTimestamp
            << "\n";


        cout
            << "T_WE(t_i):\n"
            << T_WE_point
            << "\n";
    }


    return 0;
}
