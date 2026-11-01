#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <algorithm>
/*
// This example deliberately exposes the core math, but a production AV tracker would
// upgrade several pieces
\[
\boxed{
\begin{array}{l}
\text{1. Full }T_{camera\leftarrow lidar}\text{ extrinsic calibration}\\
\text{2. Project complete 3D LiDAR boxes, not just centers}\\
\text{3. 2D/3D IoU + appearance embeddings}\\
\text{4. Hungarian / JPDA / MHT instead of greedy assignment}\\
\text{5. Ego-motion compensation}\\
\text{6. Asynchronous camera/LiDAR timestamps}\\
\text{7. EKF/UKF for nonlinear camera measurements}\\
\text{8. CV/CA/CTRV IMM motion models}\\
\text{9. Proper }Q(dt)\text{ from acceleration-noise model}\\
\text{10. Joseph-form covariance update}\\
\text{11. Class probability history}\\
\text{12. Occlusion reasoning}
\end{array}
}
\]


That is the direction I would take for a real autonomous-driving tracker: 
one persistent 3D state, LiDAR measurement model \(h_L(x)\), camera measurement 
model \(h_C(x)\), asynchronous updates, ego-motion compensation, covariance gating,
and JPDA/MHT association rather than forcing every camera frame to have a paired
LiDAR detection.


*/
using namespace std;

// ============================================================
// 1. Basic structures
// ============================================================

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
};

struct CameraDetection {
    int id;

    // 2D bounding box
    double umin;
    double vmin;
    double umax;
    double vmax;

    string objectClass;
    double confidence;

    double centerU() const {
        return (umin + umax) / 2.0;
    }

    double centerV() const {
        return (vmin + vmax) / 2.0;
    }
};

struct LidarDetection {
    int id;
    // Position in ego frame
    double x;
    double y;
    double z;
    // 3D bounding-box dimensions
    double length;
    double width;
    double height;

    double yaw;

    string objectClass;
    double confidence;
};

struct FusedDetection {
    double x;
    double y;
    double z;

    double length;
    double width;
    double height;
    double yaw;

    string objectClass;
    double confidence;

    int cameraId;
    int lidarId;
};

// ============================================================
// 2. Camera model
// ============================================================

struct CameraIntrinsics {
    double fx;
    double fy;
    double cx;
    double cy;
};

// ------------------------------------------------------------
// Simplified ego -> camera transform
//
// Ego:
//     x = forward
//     y = left
//     z = up
//
// Camera:
//     X = right
//     Y = down
//     Z = forward
//
// Therefore:
//
// Xc = -y_ego
// Yc = -z_ego
// Zc =  x_ego
//
// Depending on your convention, the sign of y may differ.
// ------------------------------------------------------------

Vec2 projectLidarToCamera(
    const LidarDetection& lidar,
    const CameraIntrinsics& K)
{
    double Xc = -lidar.y;
    double Yc = -lidar.z;
    double Zc =  lidar.x;

    if (Zc <= 0.1) {
        return {-1, -1};
    }

    double u = K.fx * Xc / Zc + K.cx;
    double v = K.fy * Yc / Zc + K.cy;

    return {u, v};
}

// ============================================================
// 3. Camera/LiDAR association
// ============================================================

double imageDistance(
    const CameraDetection& cam,
    const Vec2& projectedLidar)
{
    double du = cam.centerU() - projectedLidar.x;
    double dv = cam.centerV() - projectedLidar.y;

    return sqrt(du * du + dv * dv);
}

vector<FusedDetection> fuseCameraLidar(
    const vector<CameraDetection>& cameras,
    const vector<LidarDetection>& lidars,
    const CameraIntrinsics& K)
{
    vector<FusedDetection> result;

    vector<bool> cameraUsed(cameras.size(), false);

    for (const auto& lidar : lidars) {

        Vec2 pixel = projectLidarToCamera(lidar, K);

        cout << "\nLiDAR detection " << lidar.id
             << " projected to pixel = ("
             << pixel.x << ", "
             << pixel.y << ")\n";

        int bestCamera = -1;
        double bestDistance =
            numeric_limits<double>::max();

        for (size_t i = 0; i < cameras.size(); ++i) {
            if (cameraUsed[i])
                continue;
            // Class gating
            if (cameras[i].objectClass != lidar.objectClass)
                continue;
            double d =
                imageDistance(cameras[i], pixel);
            cout << "  distance to camera detection "
                 << cameras[i].id
                 << " = "
                 << d
                 << " pixels\n";

            if (d < bestDistance) {
                bestDistance = d;
                bestCamera = static_cast<int>(i);
            }
        }
        // Pixel gate
        const double MAX_PIXEL_DISTANCE = 80.0;
        if (bestCamera >= 0 &&
            bestDistance < MAX_PIXEL_DISTANCE)
        {
            const auto& cam = cameras[bestCamera];
            cameraUsed[bestCamera] = true;
            FusedDetection f;
            // Metric geometry comes primarily from LiDAR
            f.x = lidar.x;
            f.y = lidar.y;
            f.z = lidar.z;

            f.length = lidar.length;
            f.width  = lidar.width;
            f.height = lidar.height;
            f.yaw    = lidar.yaw;

            // Semantic class from camera
            f.objectClass = cam.objectClass;

            // Simple confidence combination
            f.confidence =
                1.0 -
                (1.0 - cam.confidence) *
                (1.0 - lidar.confidence);

            f.cameraId = cam.id;
            f.lidarId  = lidar.id;

            result.push_back(f);

            cout << "  MATCHED with Camera "
                 << cam.id
                 << "\n";
        }
    }

    return result;
}

// ============================================================
// 4. Simple 2D Kalman Filter
//
// State:
//
// x = [px, py, vx, vy]
//
// Measurement:
//
// z = [px, py]
//
// ============================================================

class KalmanFilter2D {

public:

    // state
    double x[4];
    // covariance
    double P[4][4];
    // process noise
    double q = 0.5;
    // measurement noise
    double r = 0.25;
    KalmanFilter2D(
        double px,
        double py)
    {
        x[0] = px;
        x[1] = py;

        x[2] = 0.0;
        x[3] = 0.0;

        // Initial covariance
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                P[i][j] = 0.0;

        P[0][0] = 1.0;
        P[1][1] = 1.0;

        // High initial velocity uncertainty
        P[2][2] = 25.0;
        P[3][3] = 25.0;
    }

    // --------------------------------------------------------
    // Prediction
    //
    // F =
    //
    // [1 0 dt 0 ]
    // [0 1 0  dt]
    // [0 0 1  0 ]
    // [0 0 0  1 ]
    //
    // --------------------------------------------------------

    void predict(double dt)
    {
        // State prediction

        x[0] += x[2] * dt;
        x[1] += x[3] * dt;

        double F[4][4] = {
            {1, 0, dt, 0},
            {0, 1, 0, dt},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };

        // temp = F * P

        double temp[4][4] = {};

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    temp[i][j] +=
                        F[i][k] * P[k][j];

        // newP = temp * F^T
        double newP[4][4] = {};

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    newP[i][j] +=
                        temp[i][k] * F[j][k];

        // Simple process noise
        for (int i = 0; i < 4; ++i)
            newP[i][i] += q;

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                P[i][j] = newP[i][j];
    }

    // --------------------------------------------------------
    // Measurement update
    //
    // z = [px, py]
    //
    // H =
    //
    // [1 0 0 0]
    // [0 1 0 0]
    //
    // --------------------------------------------------------

    void update(
        double measuredX,
        double measuredY)
    {
        // Innovation

        double innovation[2];

        innovation[0] =
            measuredX - x[0];

        innovation[1] =
            measuredY - x[1];

        // S = HPH' + R
        //
        // Since H simply selects px,py:
        double S00 = P[0][0] + r;
        double S01 = P[0][1];
        double S10 = P[1][0];
        double S11 = P[1][1] + r;

        // inverse of 2x2 S
        double det =
            S00 * S11 -
            S01 * S10;

        double invS[2][2];
        invS[0][0] =  S11 / det;
        invS[0][1] = -S01 / det;
        invS[1][0] = -S10 / det;
        invS[1][1] =  S00 / det;

        // K = P H' S^-1
        // P H' = first two columns of P
        double K[4][2];
        for (int i = 0; i < 4; ++i) {

            K[i][0] =
                P[i][0] * invS[0][0] +
                P[i][1] * invS[1][0];

            K[i][1] =
                P[i][0] * invS[0][1] +
                P[i][1] * invS[1][1];
        }

        // State update
        for (int i = 0; i < 4; ++i) {
            x[i] +=
                K[i][0] * innovation[0] +
                K[i][1] * innovation[1];
        }

        // P = (I-KH)P
        double oldP[4][4];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                oldP[i][j] = P[i][j];

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                P[i][j] =
                    oldP[i][j]
                    - K[i][0] * oldP[0][j]
                    - K[i][1] * oldP[1][j];
            }
        }
    }

    // --------------------------------------------------------
    // Mahalanobis distance for track association
    // --------------------------------------------------------

    double mahalanobis(
        double measuredX,
        double measuredY)
    {
        double dx =
            measuredX - x[0];
        double dy =
            measuredY - x[1];

        double S00 = P[0][0] + r;
        double S01 = P[0][1];
        double S10 = P[1][0];
        double S11 = P[1][1] + r;

        double det =
            S00 * S11 -
            S01 * S10;

        double inv00 =  S11 / det;
        double inv01 = -S01 / det;
        double inv10 = -S10 / det;
        double inv11 =  S00 / det;

        return
            dx * (inv00 * dx + inv01 * dy)
            +
            dy * (inv10 * dx + inv11 * dy);
    }
};

// ============================================================
// 5. Track structure
// ============================================================

struct Track {

    int id;
    string objectClass;
    KalmanFilter2D kf;

    double length;
    double width;
    double height;

    int age;
    int hits;
    int misses;

    bool confirmed;

    Track(
        int id_,
        const FusedDetection& detection)
        :
        id(id_),
        objectClass(detection.objectClass),
        kf(detection.x, detection.y)
    {
        length = detection.length;
        width  = detection.width;
        height = detection.height;

        age = 1;
        hits = 1;
        misses = 0;

        confirmed = false;
    }
};

// ============================================================
// 6. Track manager
// ============================================================

class Tracker {

private:

    vector<Track> tracks;
    int nextTrackId = 1;

public:

    void process(
        const vector<FusedDetection>& detections,
        double dt)
    {
        // ----------------------------------------------------
        // STEP 1:
        // Predict all existing tracks
        // ----------------------------------------------------

        for (auto& track : tracks) {

            track.kf.predict(dt);
            track.age++;

            cout << "\nPredicted Track "
                 << track.id
                 << ": x="
                 << track.kf.x[0]
                 << " y="
                 << track.kf.x[1]
                 << " vx="
                 << track.kf.x[2]
                 << " vy="
                 << track.kf.x[3]
                 << "\n";
        }

        // ----------------------------------------------------
        // STEP 2:
        // Greedy association for demonstration.
        //
        // Production system:
        // Hungarian / JPDA / MHT.
        // ----------------------------------------------------

        vector<bool> detectionUsed(
            detections.size(),
            false);

        for (auto& track : tracks) {

            int bestDetection = -1;

            double bestDistance =
                numeric_limits<double>::max();

            for (size_t j = 0;
                 j < detections.size();
                 ++j)
            {
                if (detectionUsed[j])
                    continue;

                if (track.objectClass !=
                    detections[j].objectClass)
                    continue;

                double d2 =
                    track.kf.mahalanobis(
                        detections[j].x,
                        detections[j].y);

                cout
                    << "Track "
                    << track.id
                    << " -> Detection "
                    << j
                    << " Mahalanobis d^2 = "
                    << d2
                    << "\n";

                if (d2 < bestDistance) {

                    bestDistance = d2;
                    bestDetection =
                        static_cast<int>(j);
                }
            }

            // 2D 95% chi-square gate
            //
            // chi-square(2,0.95) ≈ 5.991

            const double GATE = 5.991;

            if (bestDetection >= 0 &&
                bestDistance < GATE)
            {
                const auto& d =
                    detections[bestDetection];

                track.kf.update(
                    d.x,
                    d.y);

                track.length = d.length;
                track.width  = d.width;
                track.height = d.height;

                track.hits++;
                track.misses = 0;

                // Confirm after 3 hits
                if (track.hits >= 3)
                    track.confirmed = true;

                detectionUsed[bestDetection] =
                    true;

                cout
                    << "Track "
                    << track.id
                    << " matched.\n";
            }
            else {

                track.misses++;

                cout
                    << "Track "
                    << track.id
                    << " missed.\n";
            }
        }

        // ----------------------------------------------------
        // STEP 3:
        // Create tracks from unmatched detections
        // ----------------------------------------------------

        for (size_t j = 0;
             j < detections.size();
             ++j)
        {
            if (!detectionUsed[j]) {

                tracks.emplace_back(
                    nextTrackId++,
                    detections[j]);

                cout
                    << "Created new track "
                    << tracks.back().id
                    << "\n";
            }
        }

        // ----------------------------------------------------
        // STEP 4:
        // Delete stale tracks
        // ----------------------------------------------------

        tracks.erase(
            remove_if(
                tracks.begin(),
                tracks.end(),
                [](const Track& t)
                {
                    return t.misses > 3;
                }),
            tracks.end());
    }

    void printTracks() const
    {
        cout
            << "\n==============================\n"
            << "TRACKS\n"
            << "==============================\n";

        for (const auto& t : tracks) {

            cout
                << "Track ID: "
                << t.id
                << "\n";

            cout
                << "Class: "
                << t.objectClass
                << "\n";

            cout
                << "Position: ("
                << t.kf.x[0]
                << ", "
                << t.kf.x[1]
                << ")\n";

            cout
                << "Velocity: ("
                << t.kf.x[2]
                << ", "
                << t.kf.x[3]
                << ")\n";

            cout
                << "Hits: "
                << t.hits
                << "\n";

            cout
                << "Misses: "
                << t.misses
                << "\n";

            cout
                << "Status: "
                << (t.confirmed ?
                    "CONFIRMED" :
                    "TENTATIVE")
                << "\n\n";
        }
    }
};

// ============================================================
// 7. Main numerical example
// ============================================================

int main()
{
    cout << fixed << setprecision(3);

    CameraIntrinsics K;

    K.fx = 1000.0;
    K.fy = 1000.0;

    K.cx = 960.0;
    K.cy = 540.0;

    Tracker tracker;

    // ========================================================
    // FRAME 1
    // ========================================================

    cout << "\n\n========== FRAME 1 ==========\n";

    vector<LidarDetection> lidar1 = {

        {
            1,
            20.0, -2.0, 0.0,
            4.5, 1.8, 1.5,
            0.0,
            "car",
            0.95
        },

        {
            2,
            30.0, 3.0, 0.0,
            4.7, 1.9, 1.6,
            0.0,
            "car",
            0.93
        }
    };

    // LiDAR 1:
    //
    // Xc = 2
    // Zc = 20
    //
    // u = 1000*(2/20)+960
    //   = 1060
    //
    // LiDAR 2:
    //
    // Xc = -3
    // Zc = 30
    //
    // u = 1000*(-3/30)+960
    //   = 860

    vector<CameraDetection> camera1 = {

        {
            101,
            1010, 490,
            1110, 590,
            "car",
            0.97
        },

        {
            102,
            810, 490,
            910, 590,
            "car",
            0.96
        }
    };

    auto fused1 =
        fuseCameraLidar(
            camera1,
            lidar1,
            K);

    cout
        << "\nNumber of fused detections = "
        << fused1.size()
        << "\n";

    tracker.process(
        fused1,
        0.1);

    tracker.printTracks();

    // ========================================================
    // FRAME 2
    //
    // 100 ms later.
    //
    // Car 1 moved:
    //
    // 20.0 -> 21.0
    //
    // approximately 10 m/s
    //
    // Car 2:
    //
    // 30.0 -> 30.5
    //
    // approximately 5 m/s
    //
    // ========================================================

    cout << "\n\n========== FRAME 2 ==========\n";

    vector<LidarDetection> lidar2 = {

        {
            3,
            21.0, -2.0, 0.0,
            4.5, 1.8, 1.5,
            0.0,
            "car",
            0.96
        },

        {
            4,
            30.5, 3.0, 0.0,
            4.7, 1.9, 1.6,
            0.0,
            "car",
            0.94
        }
    };

    // Expected projection:
    //
    // car 1:
    //
    // u =
    // 1000*(2/21)+960
    // ≈ 1055.24
    //
    // car 2:
    //
    // u =
    // 1000*(-3/30.5)+960
    // ≈ 861.64

    vector<CameraDetection> camera2 = {

        {
            103,
            1005, 490,
            1105, 590,
            "car",
            0.98
        },

        {
            104,
            812, 490,
            912, 590,
            "car",
            0.95
        }
    };

    auto fused2 =
        fuseCameraLidar(
            camera2,
            lidar2,
            K);

    tracker.process(
        fused2,
        0.1);

    tracker.printTracks();

    // ========================================================
    // FRAME 3
    // ========================================================

    cout << "\n\n========== FRAME 3 ==========\n";

    vector<LidarDetection> lidar3 = {

        {
            5,
            22.0, -2.0, 0.0,
            4.5, 1.8, 1.5,
            0.0,
            "car",
            0.96
        },

        {
            6,
            31.0, 3.0, 0.0,
            4.7, 1.9, 1.6,
            0.0,
            "car",
            0.94
        }
    };

    vector<CameraDetection> camera3 = {

        {
            105,
            1000, 490,
            1100, 590,
            "car",
            0.98
        },

        {
            106,
            813, 490,
            913, 590,
            "car",
            0.96
        }
    };

    auto fused3 =
        fuseCameraLidar(
            camera3,
            lidar3,
            K);

    tracker.process(
        fused3,
        0.1);

    tracker.printTracks();

    return 0;
}
