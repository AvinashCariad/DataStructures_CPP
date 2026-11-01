#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace Eigen;

// ============================================================
// Autonomous-driving camera + LiDAR tracker
//
// Persistent state in WORLD frame:
//
// x = [X, Y, Z, Vx, Vy, Vz]^T
//
// Features:
//   * asynchronous timestamps
//   * ego-motion compensation
//   * LiDAR measurement model hL(x)
//   * camera nonlinear measurement model hC(x)
//   * EKF
//   * Mahalanobis gating
//   * simplified JPDA
//   * track lifecycle
// ============================================================

constexpr int NX = 6;

using StateVector = Matrix<double, NX, 1>;
using StateMatrix = Matrix<double, NX, NX>;


// ============================================================
// 1. Sensor measurements
// ============================================================

struct LidarMeasurement
{
    double timestamp;

    // Measurement in LiDAR/ego frame
    Vector3d position;

    int objectClass;

    // measurement covariance
    Matrix3d R;
};


struct CameraMeasurement
{
    double timestamp;

    // pixel center
    Vector2d pixel;

    int objectClass;

    double confidence;

    // measurement covariance in pixels
    Matrix2d R;
};


// ============================================================
// 2. Camera model
// ============================================================

struct CameraModel
{
    double fx;
    double fy;
    double cx;
    double cy;

    // Camera pose relative to ego vehicle.
    //
    // p_camera =
    // R_camera_ego * p_ego + t_camera_ego

    Matrix3d R_camera_ego;
    Vector3d t_camera_ego;
};


// ============================================================
// 3. Ego pose
//
// We assume localization provides:
//
// T_world_ego
//
// p_world = R_world_ego * p_ego + t_world_ego
// ============================================================

struct EgoPose
{
    double timestamp;

    Matrix3d R_world_ego;
    Vector3d t_world_ego;
};


// ============================================================
// 4. Coordinate transformations
// ============================================================

Vector3d egoToWorld(
    const Vector3d& pEgo,
    const EgoPose& ego)
{
    return ego.R_world_ego * pEgo
         + ego.t_world_ego;
}


Vector3d worldToEgo(
    const Vector3d& pWorld,
    const EgoPose& ego)
{
    return ego.R_world_ego.transpose()
         * (pWorld - ego.t_world_ego);
}


Vector3d worldToCamera(
    const Vector3d& pWorld,
    const EgoPose& ego,
    const CameraModel& camera)
{
    // World -> Ego

    Vector3d pEgo =
        worldToEgo(
            pWorld,
            ego);

    // Ego -> Camera

    Vector3d pCamera =
        camera.R_camera_ego * pEgo
        + camera.t_camera_ego;

    return pCamera;
}


// ============================================================
// 5. Track
// ============================================================

class Track
{
public:

    int id;

    int objectClass;

    StateVector x;

    StateMatrix P;

    double timestamp;

    int hits = 1;
    int misses = 0;

    bool confirmed = false;


    Track(
        int id_,
        int objectClass_,
        const Vector3d& initialPosition,
        double time)
        :
        id(id_),
        objectClass(objectClass_),
        timestamp(time)
    {
        x.setZero();

        x(0) = initialPosition.x();
        x(1) = initialPosition.y();
        x(2) = initialPosition.z();

        // Velocity initially unknown.

        x(3) = 0.0;
        x(4) = 0.0;
        x(5) = 0.0;


        P.setZero();

        // position uncertainty

        P(0,0) = 1.0;
        P(1,1) = 1.0;
        P(2,2) = 1.0;

        // velocity highly uncertain

        P(3,3) = 25.0;
        P(4,4) = 25.0;
        P(5,5) = 25.0;
    }


    // ========================================================
    // 6. Prediction
    //
    // Constant velocity model
    //
    // X' = X + Vx dt
    // Y' = Y + Vy dt
    // Z' = Z + Vz dt
    // ========================================================

    void predict(double newTimestamp)
    {
        double dt =
            newTimestamp - timestamp;

        if (dt <= 0.0)
            return;


        StateMatrix F =
            StateMatrix::Identity();

        F(0,3) = dt;
        F(1,4) = dt;
        F(2,5) = dt;


        // State prediction

        x = F * x;


        // -----------------------------------------------
        // Process noise
        //
        // White acceleration noise model
        // -----------------------------------------------

        double sigmaA = 2.0;

        double dt2 = dt * dt;
        double dt3 = dt2 * dt;
        double dt4 = dt2 * dt2;


        StateMatrix Q =
            StateMatrix::Zero();


        for (int axis = 0;
             axis < 3;
             ++axis)
        {
            int p = axis;
            int v = axis + 3;

            Q(p,p) =
                0.25 * dt4
                * sigmaA * sigmaA;

            Q(p,v) =
                0.5 * dt3
                * sigmaA * sigmaA;

            Q(v,p) =
                Q(p,v);

            Q(v,v) =
                dt2
                * sigmaA * sigmaA;
        }


        P =
            F * P * F.transpose()
            + Q;


        timestamp = newTimestamp;
    }


    Vector3d position() const
    {
        return x.head<3>();
    }


    Vector3d velocity() const
    {
        return x.tail<3>();
    }
};


// ============================================================
// 7. LiDAR measurement model
//
// Track lives in WORLD coordinates.
//
// LiDAR measurement has already been ego-motion compensated
// and transformed into WORLD coordinates.
//
// hL(x) =
//
// [ X ]
// [ Y ]
// [ Z ]
//
// ============================================================

Vector3d lidarMeasurementModel(
    const Track& track)
{
    return track.x.head<3>();
}


Matrix<double,3,NX>
lidarJacobian()
{
    Matrix<double,3,NX> H;

    H.setZero();

    H(0,0) = 1.0;
    H(1,1) = 1.0;
    H(2,2) = 1.0;

    return H;
}


// ============================================================
// 8. Camera measurement model
//
// State:
//
// position in WORLD frame.
//
// First:
//
// World -> Ego -> Camera
//
// Then:
//
// u = fx * Xc/Zc + cx
// v = fy * Yc/Zc + cy
//
// ============================================================

Vector2d cameraMeasurementModel(
    const Track& track,
    const EgoPose& ego,
    const CameraModel& camera)
{
    Vector3d pWorld =
        track.position();

    Vector3d pCamera =
        worldToCamera(
            pWorld,
            ego,
            camera);


    double X = pCamera.x();
    double Y = pCamera.y();
    double Z = pCamera.z();


    if (Z <= 0.1)
    {
        return Vector2d(
            numeric_limits<double>::quiet_NaN(),
            numeric_limits<double>::quiet_NaN());
    }


    double u =
        camera.fx * X / Z
        + camera.cx;

    double v =
        camera.fy * Y / Z
        + camera.cy;


    return Vector2d(u,v);
}


// ============================================================
// 9. Camera Jacobian
//
// dh/dp_camera:
//
// [ fx/Z      0      -fx*X/Z² ]
// [ 0        fy/Z    -fy*Y/Z² ]
//
// Need:
//
// dp_camera / dp_world
//
// p_ego = R_world_ego^T * (...)
//
// p_cam = R_camera_ego * p_ego
//
// Therefore:
//
// dp_camera/dp_world
//
// = R_camera_ego * R_world_ego^T
//
// ============================================================

Matrix<double,2,NX>
cameraJacobian(
    const Track& track,
    const EgoPose& ego,
    const CameraModel& camera)
{
    Vector3d pCamera =
        worldToCamera(
            track.position(),
            ego,
            camera);


    double X = pCamera.x();
    double Y = pCamera.y();
    double Z = pCamera.z();


    Matrix<double,2,3> Jproj;

    Jproj <<
        camera.fx / Z,
        0.0,
        -camera.fx * X / (Z * Z),

        0.0,
        camera.fy / Z,
        -camera.fy * Y / (Z * Z);


    Matrix3d JworldCamera =
        camera.R_camera_ego
        * ego.R_world_ego.transpose();


    Matrix<double,2,NX> H;

    H.setZero();

    H.block<2,3>(0,0) =
        Jproj * JworldCamera;


    // Camera directly observes position,
    // not velocity.

    return H;
}


// ============================================================
// 10. Generic EKF update
// ============================================================

template<int NZ>
void ekfUpdate(
    Track& track,
    const Matrix<double,NZ,1>& z,
    const Matrix<double,NZ,1>& zPred,
    const Matrix<double,NZ,NX>& H,
    const Matrix<double,NZ,NZ>& R)
{
    Matrix<double,NZ,1> innovation =
        z - zPred;


    Matrix<double,NZ,NZ> S =
        H * track.P * H.transpose()
        + R;


    Matrix<double,NX,NZ> K =
        track.P
        * H.transpose()
        * S.inverse();


    track.x =
        track.x
        + K * innovation;


    // Joseph covariance form:
    //
    // P =
    // (I-KH)P(I-KH)' + KRK'

    StateMatrix I =
        StateMatrix::Identity();

    StateMatrix A =
        I - K * H;


    track.P =
        A * track.P * A.transpose()
        + K * R * K.transpose();
}


// ============================================================
// 11. Mahalanobis distance
// ============================================================

template<int NZ>
double mahalanobisSquared(
    const Matrix<double,NZ,1>& innovation,
    const Matrix<double,NZ,NZ>& S)
{
    return
        innovation.transpose()
        * S.inverse()
        * innovation;
}


// ============================================================
// 12. LiDAR gating
// ============================================================

double lidarGateDistance(
    const Track& track,
    const Vector3d& measurement,
    const Matrix3d& R)
{
    auto H =
        lidarJacobian();

    Vector3d predicted =
        lidarMeasurementModel(track);

    Vector3d innovation =
        measurement - predicted;


    Matrix3d S =
        H * track.P * H.transpose()
        + R;


    return
        mahalanobisSquared<3>(
            innovation,
            S);
}


// ============================================================
// 13. Camera gating
// ============================================================

double cameraGateDistance(
    const Track& track,
    const CameraMeasurement& measurement,
    const EgoPose& ego,
    const CameraModel& camera)
{
    Vector2d predicted =
        cameraMeasurementModel(
            track,
            ego,
            camera);


    if (!predicted.allFinite())
        return numeric_limits<double>::infinity();


    auto H =
        cameraJacobian(
            track,
            ego,
            camera);


    Vector2d innovation =
        measurement.pixel
        - predicted;


    Matrix2d S =
        H * track.P * H.transpose()
        + measurement.R;


    return
        mahalanobisSquared<2>(
            innovation,
            S);
}


// ============================================================
// 14. Gaussian likelihood
//
// Needed for JPDA.
//
// Ignoring constants shared by candidates:
//
// L ~ exp(-0.5 d²) / sqrt(det(S))
//
// ============================================================

template<int NZ>
double gaussianLikelihood(
    const Matrix<double,NZ,1>& innovation,
    const Matrix<double,NZ,NZ>& S)
{
    double d2 =
        mahalanobisSquared<NZ>(
            innovation,
            S);


    double denominator =
        sqrt(
            pow(2.0 * M_PI, NZ)
            * S.determinant());


    return
        exp(-0.5 * d2)
        / denominator;
}


// ============================================================
// 15. Simplified JPDA LiDAR update
//
// For one track, multiple measurements can lie inside
// its validation gate.
//
// beta0 = missed detection probability
//
// beta_j = probability measurement j belongs to track.
//
// NOTE:
//
// Full multi-target JPDA computes probabilities over
// JOINT feasible association events.
//
// This implementation demonstrates the numerical
// marginal-update concept.
// ============================================================

void jpdaLidarUpdate(
    Track& track,
    const vector<Vector3d>& measurements,
    const Matrix3d& R)
{
    auto H =
        lidarJacobian();


    Vector3d predicted =
        lidarMeasurementModel(track);


    Matrix3d S =
        H * track.P * H.transpose()
        + R;


    // 99% chi-square gate for 3 DOF
    //
    // approximately 11.345

    const double gate = 11.345;


    struct Candidate
    {
        Vector3d measurement;
        Vector3d innovation;
        double likelihood;
    };


    vector<Candidate> candidates;


    for (const auto& z : measurements)
    {
        Vector3d innovation =
            z - predicted;


        double d2 =
            mahalanobisSquared<3>(
                innovation,
                S);


        if (d2 < gate)
        {
            double likelihood =
                gaussianLikelihood<3>(
                    innovation,
                    S);


            candidates.push_back(
                {
                    z,
                    innovation,
                    likelihood
                });
        }
    }


    if (candidates.empty())
    {
        track.misses++;

        return;
    }


    // --------------------------------------------------------
    // Simplified missed detection weight
    //
    // In production JPDA this comes from:
    //
    // detection probability,
    // clutter density,
    // joint association events, etc.
    // --------------------------------------------------------

    double missedWeight = 0.05;


    double normalization =
        missedWeight;


    for (const auto& c : candidates)
        normalization += c.likelihood;


    double beta0 =
        missedWeight / normalization;


    Vector3d weightedInnovation =
        Vector3d::Zero();


    cout << "\nJPDA Track "
         << track.id
         << "\n";

    cout << "beta0(missed) = "
         << beta0
         << "\n";


    for (size_t j = 0;
         j < candidates.size();
         ++j)
    {
        double beta =
            candidates[j].likelihood
            / normalization;


        cout
            << "beta measurement "
            << j
            << " = "
            << beta
            << "\n";


        weightedInnovation +=
            beta
            * candidates[j].innovation;
    }


    // Kalman gain

    Matrix<double,NX,3> K =
        track.P
        * H.transpose()
        * S.inverse();


    track.x +=
        K * weightedInnovation;


    // --------------------------------------------------------
    // Approximate JPDA covariance correction
    //
    // First calculate spread of possible innovations.
    // --------------------------------------------------------

    Matrix3d spread =
        Matrix3d::Zero();


    for (const auto& c : candidates)
    {
        double beta =
            c.likelihood
            / normalization;


        spread +=
            beta
            * c.innovation
            * c.innovation.transpose();
    }


    spread -=
        weightedInnovation
        * weightedInnovation.transpose();


    StateMatrix normalKFReduction =
        K * S * K.transpose();


    track.P =
        track.P
        - (1.0 - beta0)
        * normalKFReduction
        + K * spread * K.transpose();


    track.hits++;
    track.misses = 0;

    if (track.hits >= 3)
        track.confirmed = true;
}


// ============================================================
// 16. Camera EKF update
// ============================================================

void cameraUpdate(
    Track& track,
    const CameraMeasurement& measurement,
    const EgoPose& ego,
    const CameraModel& camera)
{
    Vector2d predicted =
        cameraMeasurementModel(
            track,
            ego,
            camera);


    if (!predicted.allFinite())
        return;


    auto H =
        cameraJacobian(
            track,
            ego,
            camera);


    // gating

    Vector2d innovation =
        measurement.pixel
        - predicted;


    Matrix2d S =
        H * track.P * H.transpose()
        + measurement.R;


    double d2 =
        mahalanobisSquared<2>(
            innovation,
            S);


    // 99% chi-square, 2 DOF
    //
    // 9.21

    if (d2 > 9.21)
    {
        cout
            << "Camera measurement rejected. d2="
            << d2
            << "\n";

        return;
    }


    cout
        << "Camera update accepted. d2="
        << d2
        << "\n";


    ekfUpdate<2>(
        track,
        measurement.pixel,
        predicted,
        H,
        measurement.R);
}


// ============================================================
// 17. Track manager
// ============================================================

class Tracker
{
private:

    vector<Track> tracks;

    int nextTrackId = 1;


public:

    // --------------------------------------------------------
    // Initialize from LiDAR.
    //
    // This is convenient because LiDAR gives metric 3D
    // position directly.
    // --------------------------------------------------------

    void createTrack(
        const LidarMeasurement& measurement,
        const EgoPose& ego)
    {
        Vector3d worldPosition =
            egoToWorld(
                measurement.position,
                ego);


        tracks.emplace_back(
            nextTrackId++,
            measurement.objectClass,
            worldPosition,
            measurement.timestamp);


        cout
            << "Created Track "
            << tracks.back().id
            << " at world position "
            << worldPosition.transpose()
            << "\n";
    }


    // ========================================================
    // Asynchronous LiDAR event
    // ========================================================

    void processLidar(
        const vector<LidarMeasurement>& measurements,
        const EgoPose& ego)
    {
        if (measurements.empty())
            return;


        double time =
            measurements[0].timestamp;


        cout
            << "\n================================\n"
            << "LiDAR event @ "
            << time
            << " sec\n"
            << "================================\n";


        // ----------------------------------------------------
        // 1. Predict every track exactly to LiDAR timestamp
        // ----------------------------------------------------

        for (auto& track : tracks)
            track.predict(time);


        // ----------------------------------------------------
        // 2. Ego-motion compensation
        //
        // Measurements are currently ego-frame.
        //
        // Transform them into world coordinates.
        // ----------------------------------------------------

        vector<Vector3d> worldMeasurements;


        for (const auto& m : measurements)
        {
            worldMeasurements.push_back(
                egoToWorld(
                    m.position,
                    ego));
        }


        // ----------------------------------------------------
        // First frame: create tracks
        // ----------------------------------------------------

        if (tracks.empty())
        {
            for (const auto& m : measurements)
                createTrack(m, ego);

            return;
        }


        // ----------------------------------------------------
        // 3. JPDA
        //
        // For demonstration each track sees all measurements.
        // Mahalanobis gating inside JPDA removes implausible
        // measurements.
        // ----------------------------------------------------

        for (auto& track : tracks)
        {
            Matrix3d R =
                Matrix3d::Identity()
                * 0.25;


            jpdaLidarUpdate(
                track,
                worldMeasurements,
                R);
        }


        // ----------------------------------------------------
        // 4. Delete stale tracks
        // ----------------------------------------------------

        tracks.erase(
            remove_if(
                tracks.begin(),
                tracks.end(),
                [](const Track& t)
                {
                    return t.misses > 5;
                }),
            tracks.end());
    }


    // ========================================================
    // Asynchronous camera event
    // ========================================================

    void processCamera(
        const vector<CameraMeasurement>& measurements,
        const EgoPose& ego,
        const CameraModel& camera)
    {
        if (measurements.empty())
            return;


        double time =
            measurements[0].timestamp;


        cout
            << "\n================================\n"
            << "Camera event @ "
            << time
            << " sec\n"
            << "================================\n";


        // Predict tracks to camera timestamp

        for (auto& track : tracks)
            track.predict(time);


        // ----------------------------------------------------
        // Simplified camera association:
        //
        // Find minimum Mahalanobis distance.
        //
        // The same candidate-generation machinery can feed
        // JPDA or MHT instead.
        // ----------------------------------------------------

        vector<bool> used(
            measurements.size(),
            false);


        for (auto& track : tracks)
        {
            int best = -1;

            double bestD2 =
                numeric_limits<double>::infinity();


            for (size_t j = 0;
                 j < measurements.size();
                 ++j)
            {
                if (used[j])
                    continue;


                if (measurements[j].objectClass
                    != track.objectClass)
                    continue;


                double d2 =
                    cameraGateDistance(
                        track,
                        measurements[j],
                        ego,
                        camera);


                cout
                    << "Track "
                    << track.id
                    << " -> Camera "
                    << j
                    << " d2 = "
                    << d2
                    << "\n";


                if (d2 < bestD2)
                {
                    bestD2 = d2;
                    best =
                        static_cast<int>(j);
                }
            }


            if (best >= 0 &&
                bestD2 < 9.21)
            {
                cameraUpdate(
                    track,
                    measurements[best],
                    ego,
                    camera);


                used[best] = true;
            }
        }
    }


    void printTracks() const
    {
        cout
            << "\n--------------------------------\n"
            << "TRACK STATES\n"
            << "--------------------------------\n";


        for (const auto& track : tracks)
        {
            cout
                << "Track "
                << track.id
                << "\n";


            cout
                << "  position world = "
                << track.position().transpose()
                << "\n";


            cout
                << "  velocity world = "
                << track.velocity().transpose()
                << "\n";


            cout
                << "  position sigma = "
                << sqrt(track.P(0,0))
                << ", "
                << sqrt(track.P(1,1))
                << ", "
                << sqrt(track.P(2,2))
                << "\n";


            cout
                << "  status = "
                << (track.confirmed ?
                    "CONFIRMED" :
                    "TENTATIVE")
                << "\n";
        }
    }
};


// ============================================================
// 18. Main numerical simulation
// ============================================================

int main()
{
    cout << fixed << setprecision(3);


    // ========================================================
    // Camera calibration
    // ========================================================

    CameraModel camera;

    camera.fx = 1000.0;
    camera.fy = 1000.0;

    camera.cx = 960.0;
    camera.cy = 540.0;


    // --------------------------------------------------------
    // Ego coordinates:
    //
    // X = forward
    // Y = left
    // Z = up
    //
    // Camera:
    //
    // Xc = right
    // Yc = down
    // Zc = forward
    //
    // Therefore:
    //
    // Xc = -Yego
    // Yc = -Zego
    // Zc =  Xego
    // --------------------------------------------------------

    camera.R_camera_ego <<

         0, -1,  0,
         0,  0, -1,
         1,  0,  0;


    camera.t_camera_ego =
        Vector3d::Zero();


    Tracker tracker;


    // ========================================================
    // t = 0.000
    //
    // Ego vehicle is at world x = 0.
    //
    // Object is 20 m ahead.
    // ========================================================

    EgoPose ego0;

    ego0.timestamp = 0.0;

    ego0.R_world_ego =
        Matrix3d::Identity();

    ego0.t_world_ego =
        Vector3d(0,0,0);


    LidarMeasurement lidar0;

    lidar0.timestamp = 0.0;

    lidar0.position =
        Vector3d(
            20.0,
            -2.0,
            0.0);

    lidar0.objectClass = 1;

    lidar0.R =
        Matrix3d::Identity()
        * 0.25;


    tracker.processLidar(
        {lidar0},
        ego0);


    tracker.printTracks();


    // ========================================================
    // t = 0.033
    //
    // Camera arrives BEFORE next LiDAR.
    //
    // Ego vehicle moved approximately 0.33 m.
    // ========================================================

    EgoPose ego033;

    ego033.timestamp = 0.033;

    ego033.R_world_ego =
        Matrix3d::Identity();

    ego033.t_world_ego =
        Vector3d(
            0.33,
            0.0,
            0.0);


    CameraMeasurement cam033;

    cam033.timestamp = 0.033;

    // approximate image observation

    cam033.pixel =
        Vector2d(
            1059.0,
            540.5);

    cam033.objectClass = 1;

    cam033.confidence = 0.97;

    cam033.R =
        Matrix2d::Identity()
        * 4.0;


    tracker.processCamera(
        {cam033},
        ego033,
        camera);


    tracker.printTracks();


    // ========================================================
    // t = 0.066
    //
    // Another camera event.
    // ========================================================

    EgoPose ego066;

    ego066.timestamp = 0.066;

    ego066.R_world_ego =
        Matrix3d::Identity();

    ego066.t_world_ego =
        Vector3d(
            0.66,
            0,
            0);


    CameraMeasurement cam066;

    cam066.timestamp = 0.066;

    cam066.pixel =
        Vector2d(
            1058.0,
            540.0);

    cam066.objectClass = 1;

    cam066.confidence = 0.98;

    cam066.R =
        Matrix2d::Identity()
        * 4.0;


    tracker.processCamera(
        {cam066},
        ego066,
        camera);


    // ========================================================
    // t = 0.100
    //
    // Next LiDAR arrives.
    //
    // Ego moved 1 m.
    //
    // Suppose tracked car itself moved from world
    // X = 20 to X = 21.
    //
    // Relative LiDAR position therefore remains:
    //
    // 21 - 1 = 20 m ahead.
    //
    // THIS demonstrates ego-motion compensation.
    // ========================================================

    EgoPose ego100;

    ego100.timestamp = 0.100;

    ego100.R_world_ego =
        Matrix3d::Identity();

    ego100.t_world_ego =
        Vector3d(
            1.0,
            0,
            0);


    LidarMeasurement lidar100;

    lidar100.timestamp = 0.100;

    lidar100.position =
        Vector3d(
            20.0,
            -2.0,
            0.0);

    lidar100.objectClass = 1;

    lidar100.R =
        Matrix3d::Identity()
        * 0.25;


    // Add clutter measurement for JPDA demonstration

    LidarMeasurement clutter;

    clutter.timestamp = 0.100;

    clutter.position =
        Vector3d(
            20.4,
            -1.7,
            0.1);

    clutter.objectClass = 1;

    clutter.R =
        Matrix3d::Identity()
        * 0.25;


    tracker.processLidar(
        {
            lidar100,
            clutter
        },
        ego100);


    tracker.printTracks();


    // ========================================================
    // t = 0.133
    //
    // Camera again.
    // ========================================================

    EgoPose ego133;

    ego133.timestamp = 0.133;

    ego133.R_world_ego =
        Matrix3d::Identity();

    ego133.t_world_ego =
        Vector3d(
            1.33,
            0,
            0);


    CameraMeasurement cam133;

    cam133.timestamp = 0.133;

    cam133.pixel =
        Vector2d(
            1057.0,
            540.0);

    cam133.objectClass = 1;

    cam133.confidence = 0.98;

    cam133.R =
        Matrix2d::Identity()
        * 4.0;


    tracker.processCamera(
        {cam133},
        ego133,
        camera);


    tracker.printTracks();


    return 0;
}
