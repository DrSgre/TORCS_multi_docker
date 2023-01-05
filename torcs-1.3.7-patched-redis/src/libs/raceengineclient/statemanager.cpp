#include <sw/redis++/redis++.h>

#include <chrono>
#include <thread>
#include <fstream>
#include <raceman.h>

#define SNAPSHOT_FREQUENCY 1

using namespace sw::redis;
using namespace std::chrono;

static std::ofstream OutputFile;
static int state_id = 0;
static int prev_id = -1;
extern bool do_update;

extern Redis redis;

void SaveState(tSituation *s)
{
    // If the ETCD values are different from the local ones, they are updated accordingly.
    if (std::stod(redis.get("/state/deltaTime").value()) != s->deltaTime) {
        redis.set("/state/deltaTime", std::to_string(s->deltaTime));
    }
    if (std::stod(redis.get("/state/currentTime").value()) != s->currentTime) {
        redis.set("/state/id", std::to_string(state_id));
        state_id += 1;
        redis.set("/state/currentTime", std::to_string(s->currentTime));
    }
    if (std::stoi(redis.get("/state/raceState").value()) != s->_raceState) {
        redis.set("/state/raceState", std::to_string(s->_raceState));
    }
    // If the Redis values are different from the local ones, they are updated accordingly.
    for (int i = 0; i < s->_ncars; i++) {
        // Store the infodelation related to the position with respect to the track segment.
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toStart").value()) != s->cars[i]->_trkPos.toStart) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toStart", std::to_string(s->cars[i]->_trkPos.toStart));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toMiddle").value()) != s->cars[i]->_trkPos.toMiddle) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toMiddle", std::to_string(s->cars[i]->_trkPos.toMiddle));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toRight").value()) != s->cars[i]->_trkPos.toRight) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toRight", std::to_string(s->cars[i]->_trkPos.toRight));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toLeft").value()) != s->cars[i]->_trkPos.toLeft) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toLeft", std::to_string(s->cars[i]->_trkPos.toLeft));
        }
        if (std::stoi(redis.get("/carstate/car" + std::to_string(i) + "/pos_type").value()) != s->cars[i]->_trkPos.type) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_type", std::to_string(s->cars[i]->_trkPos.type));
        }
        if (std::stoi(redis.get("/carstate/car" + std::to_string(i) + "/seg_id").value()) != s->cars[i]->_trkPos.seg->id) {
            redis.set("/carstate/car" + std::to_string(i) + "/seg_id", std::to_string(s->cars[i]->_trkPos.seg->id));
        }

        // Store the infodelation related to the global position of the car.
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_X").value()) != s->cars[i]->_pos_X) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_X", std::to_string(s->cars[i]->_pos_X));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_Y").value()) != s->cars[i]->_pos_Y) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_Y", std::to_string(s->cars[i]->_pos_Y));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_Z").value()) != s->cars[i]->_pos_Z) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_Z", std::to_string(s->cars[i]->_pos_Z));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_AX").value()) != s->cars[i]->_roll) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_AX", std::to_string(s->cars[i]->_roll));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_AY").value()) != s->cars[i]->_pitch) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_AY", std::to_string(s->cars[i]->_pitch));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_AZ").value()) != s->cars[i]->_yaw) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_AZ", std::to_string(s->cars[i]->_yaw));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_AZ").value()) != s->cars[i]->_yaw_rate) {
            redis.set("/carstate/car" + std::to_string(i) + "/vel_AZ", std::to_string(s->cars[i]->_yaw_rate));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_X").value()) != s->cars[i]->_speed_x) {
            redis.set("/carstate/car" + std::to_string(i) + "/vel_X", std::to_string(s->cars[i]->_speed_x));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_Y").value()) != s->cars[i]->_speed_y) {
            redis.set("/carstate/car" + std::to_string(i) + "/vel_Y", std::to_string(s->cars[i]->_speed_y));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_Z").value()) != s->cars[i]->_speed_z) {
            redis.set("/carstate/car" + std::to_string(i) + "/vel_Z", std::to_string(s->cars[i]->_speed_z));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/accel_X").value()) != s->cars[i]->_accel_x) {
            redis.set("/carstate/car" + std::to_string(i) + "/accel_X", std::to_string(s->cars[i]->_accel_x));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/accel_Y").value()) != s->cars[i]->_accel_y) {
            redis.set("/carstate/car" + std::to_string(i) + "/accel_Y", std::to_string(s->cars[i]->_accel_y));
        }
        if (std::stod(redis.get("/carstate/car" + std::to_string(i) + "/accel_Z").value()) != s->cars[i]->_accel_z) {
            redis.set("/carstate/car" + std::to_string(i) + "/accel_Z", std::to_string(s->cars[i]->_accel_z));
        }
    }
}

void LoadState (tRmInfo *ReInfo) {
    tSituation *s = ReInfo->s;
    double deltaTime = std::stod(redis.get("/state/deltaTime").value());
    if (deltaTime != s->deltaTime) {
        s->deltaTime = deltaTime;
    }
    double currentTime = std::stod(redis.get("/state/currentTime").value());
    if (currentTime != s->currentTime) {
        s->currentTime = currentTime;
    }
    int raceState = std::stoi(redis.get("/state/raceState").value());
    if (raceState != s->_raceState) {
        s->_raceState = raceState;
    }
    // For each car in the race, get the data related to the position with respect to the track segment and the global position of the car.
    for (int i = 0; i < s->_ncars; i++) {
        double pos_toStart = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toStart").value());
        if (pos_toStart != s->cars[i]->_trkPos.toStart) {
            s->cars[i]->_trkPos.toStart = pos_toStart;
        }
        double pos_toMiddle = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toMiddle").value());
        if (pos_toMiddle != s->cars[i]->_trkPos.toMiddle) {
            s->cars[i]->_trkPos.toMiddle = pos_toMiddle;
        }
        double pos_toRight = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toRight").value());
        if (pos_toRight != s->cars[i]->_trkPos.toRight) {
            s->cars[i]->_trkPos.toRight  = pos_toRight;
        }
        double pos_toLeft = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_toLeft").value());
        if (pos_toLeft != s->cars[i]->_trkPos.toLeft) {
            s->cars[i]->_trkPos.toLeft = pos_toLeft;
        }
        int pos_type = std::stoi(redis.get("/carstate/car" + std::to_string(i) + "/pos_type").value());
        if (pos_type != s->cars[i]->_trkPos.type) {
            s->cars[i]->_trkPos.type = pos_type;
        }
        // In order to get the correct track segment, without storing needless data, we use the segment id as reference for iterating
        // on the list of track segments.
        int seg_id = std::stoi(redis.get("/carstate/car" + std::to_string(i) + "/seg_id").value());
        while (s->cars[i]->_trkPos.seg->id != seg_id) {
            if (s->cars[i]->_trkPos.seg->id > seg_id) {
                s->cars[i]->_trkPos.seg = s->cars[i]->_trkPos.seg->prev;
            }
            else {
                s->cars[i]->_trkPos.seg = s->cars[i]->_trkPos.seg->next;
            }
        }

        double pos_X = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_X").value());
        if (pos_X != s->cars[i]->_pos_X) {
            s->cars[i]->_pos_X = pos_X;
        }
        double pos_Y = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_Y").value());
        if (pos_Y != s->cars[i]->_pos_Y) {
            s->cars[i]->_pos_Y = pos_Y;
        }
        double pos_Z = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_Z").value());
        if (pos_Z != s->cars[i]->_pos_Z) {
            s->cars[i]->_pos_Z  = pos_Z;
        }
        double pos_AX = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_AX").value());
        if (pos_AX != s->cars[i]->_roll) {
            s->cars[i]->_roll = pos_AX;
        }
        double pos_AY = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_AY").value());
        if (pos_AY != s->cars[i]->_pitch) {
            s->cars[i]->_pitch = pos_AY;
        }
        double pos_AZ = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/pos_AZ").value());
        if (pos_AZ != s->cars[i]->_yaw) {
            s->cars[i]->_yaw  = pos_AZ;
        }
        double vel_AZ = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_AZ").value());
        if (vel_AZ != s->cars[i]->_yaw_rate) {
            s->cars[i]->_yaw_rate = vel_AZ;
        }
        double vel_X = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_X").value());
        if (vel_X != s->cars[i]->_speed_x) {
            s->cars[i]->_speed_x = vel_X;
        }
        double vel_Y = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_Y").value());
        if (vel_Y != s->cars[i]->_speed_y) {
            s->cars[i]->_speed_y  = vel_Y;
        }
        double vel_Z = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/vel_Z").value());
        if (vel_Z != s->cars[i]->_speed_z) {
            s->cars[i]->_speed_z  = vel_Z;
        }
        double accel_X = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/accel_X").value());
        if (accel_X != s->cars[i]->_accel_x) {
            s->cars[i]->_accel_x = accel_X;
        }
        double accel_Y = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/accel_Y").value());
        if (accel_Y != s->cars[i]->_accel_y) {
            s->cars[i]->_accel_y  = accel_Y;
        }
        double accel_Z = std::stod(redis.get("/carstate/car" + std::to_string(i) + "/accel_Z").value());
        if (accel_Z != s->cars[i]->_accel_z) {
            s->cars[i]->_accel_z  = accel_Z;
        }

        ReInfo->_reSimItf.config(s->cars[i], ReInfo);
    }
    prev_id = state_id;
    state_id = std::stoi(redis.get("/state/id").value());
    if (prev_id != state_id) {
        OutputFile.open("temp2.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        OutputFile << "state " + std::to_string(state_id) + " data: " +
        std::to_string(s->cars[0]->_pos_X) + "," + std::to_string(s->cars[0]->_pos_Y) + "," +  std::to_string(s->cars[0]->_pos_Z) + "," + std::to_string(s->cars[0]->_roll) + "," + std::to_string(s->cars[0]->_pitch) + "," + std::to_string(s->cars[0]->_yaw) + " " +
        std::to_string(s->cars[0]->_speed_x) + "," + std::to_string(s->cars[0]->_speed_y) + "," + std::to_string(s->cars[0]->_speed_z) + "," + std::to_string(s->cars[0]->_yaw_rate) + " " +
        std::to_string(s->cars[0]->_accel_x) + "," + std::to_string(s->cars[0]->_accel_y) + "," + std::to_string(s->cars[0]->_accel_z)
        << "\n";
        OutputFile.close();
    }
}

void StartStateManager(tRmInfo* ReInfo)
{
    tSituation *s = ReInfo->s;
    if (redis.get("/distributed_game_engine").value() == "true")
    {
        s->_raceType = std::stoi(redis.get("/state/raceType").value());
        s->_totLaps = std::stoi(redis.get("/state/totLaps").value());
        s->_ncars = std::stoi(redis.get("/state/ncars").value());
        s->_maxDammage = std::stoi(redis.get("/state/maxDammage").value());
        do_update = false;
        while(s->_raceState != RM_RACE_ENDED) {
            // If the race is paused we wait without performing requests.
            if (s->_raceState == RM_RACE_PAUSED)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(SNAPSHOT_FREQUENCY));
            }
            else 
            {
                LoadState(ReInfo);
                std::this_thread::sleep_for(std::chrono::milliseconds(SNAPSHOT_FREQUENCY));
            }
        }
    }
    else
    {
        redis.set("/distributed_game_engine", "true");
        redis.set("/state/raceType", std::to_string(s->_raceType));
        redis.set("/state/totLaps", std::to_string(s->_totLaps));
        redis.set("/state/ncars", std::to_string(s->_ncars));
        redis.set("/state/maxDammage", std::to_string(s->_maxDammage));

        // Initialize game state values set multiple times, to allow for later comparisons.
        redis.set("/state/deltaTime", std::to_string(s->deltaTime));
        redis.set("/state/currentTime", std::to_string(s->currentTime));
        redis.set("/state/raceState", std::to_string(s->_raceState));
        redis.set("/state/id", std::to_string(state_id));
        // Initialize car state values, to allow for later comparisons.
        for (int i = 0; i < s->_ncars; i++) {
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toStart", std::to_string(s->cars[i]->_trkPos.toStart));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toMiddle", std::to_string(s->cars[i]->_trkPos.toMiddle));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toRight", std::to_string(s->cars[i]->_trkPos.toRight));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_toLeft", std::to_string(s->cars[i]->_trkPos.toLeft));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_type", std::to_string(s->cars[i]->_trkPos.type));
            redis.set("/carstate/car" + std::to_string(i) + "/seg_id", std::to_string(s->cars[i]->_trkPos.seg->id));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_X", std::to_string(s->cars[i]->_pos_X));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_Y", std::to_string(s->cars[i]->_pos_Y));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_Z", std::to_string(s->cars[i]->_pos_Z));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_AX", std::to_string(s->cars[i]->_roll));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_AY", std::to_string(s->cars[i]->_pitch));
            redis.set("/carstate/car" + std::to_string(i) + "/pos_AZ", std::to_string(s->cars[i]->_yaw));
            redis.set("/carstate/car" + std::to_string(i) + "/vel_AZ", std::to_string(s->cars[i]->_yaw_rate));
            redis.set("/carstate/car" + std::to_string(i) + "/vel_X", std::to_string(s->cars[i]->_speed_x));
            redis.set("/carstate/car" + std::to_string(i) + "/vel_Y", std::to_string(s->cars[i]->_speed_y));
            redis.set("/carstate/car" + std::to_string(i) + "/vel_Z", std::to_string(s->cars[i]->_speed_z));
            redis.set("/carstate/car" + std::to_string(i) + "/accel_X", std::to_string(s->cars[i]->_accel_x));
            redis.set("/carstate/car" + std::to_string(i) + "/accel_Y", std::to_string(s->cars[i]->_accel_y));
            redis.set("/carstate/car" + std::to_string(i) + "/accel_Z", std::to_string(s->cars[i]->_accel_z));
        }

        // The game state update loop is started and proceeds with a fixed frequency until the race has ended.
        while(s->_raceState != RM_RACE_ENDED) {
            // If the race is paused we wait without performing requests.
            if (s->_raceState == RM_RACE_PAUSED)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(SNAPSHOT_FREQUENCY));
            }
            else 
            {
                OutputFile.open("temp1.txt", std::fstream::in | std::fstream::out | std::fstream::app);
                OutputFile << "state " + std::to_string(state_id) + " data: " + 
                std::to_string(s->cars[0]->_pos_X) + "," + std::to_string(s->cars[0]->_pos_Y) + "," + std::to_string(s->cars[0]->_pos_Z) + "," + std::to_string(s->cars[0]->_roll) + "," + std::to_string(s->cars[0]->_pitch) + "," + std::to_string(s->cars[0]->_yaw) + " " +
                std::to_string(s->cars[0]->_speed_x) + "," + std::to_string(s->cars[0]->_speed_y) + "," + std::to_string(s->cars[0]->_speed_z) + "," + std::to_string(s->cars[0]->_yaw_rate) + " " +
                std::to_string(s->cars[0]->_accel_x) + "," + std::to_string(s->cars[0]->_accel_y) + "," + std::to_string(s->cars[0]->_accel_z)
                << "\n";
                OutputFile.close();
                SaveState(ReInfo->s);
                std::this_thread::sleep_for(std::chrono::milliseconds(SNAPSHOT_FREQUENCY));
            }
        }
        // Clean up ETCD keys, before stopping the State Manager.
        redis.del("/state/raceType");
        redis.del("/state/totLaps");
        redis.del("/state/ncars");
        redis.del("/state/maxDammage");
        redis.del("/state/deltaTime");
        redis.del("/state/currentTime");
        redis.del("/state/raceState");
        // Clean up ETCD keys, before stopping the State Manager.
        for (int i = 0; i < s->_ncars; i++) {
            redis.del("/carstate/car" + std::to_string(i) + "/pos_toStart");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_toMiddle");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_toRight");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_toLeft");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_type");
            redis.del("/carstate/car" + std::to_string(i) + "/seg_id");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_X");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_Y");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_Z");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_AX");
            redis.del("/carstate/car" + std::to_string(i) + "/pos_AY");
            redis.del("/carstate/car" + std::to_string(i) + "/seg_AZ");
            redis.del("/carstate/car" + std::to_string(i) + "/vel_AZ");
            redis.del("/carstate/car" + std::to_string(i) + "/vel_X");
            redis.del("/carstate/car" + std::to_string(i) + "/vel_Y");
            redis.del("/carstate/car" + std::to_string(i) + "/vel_Z");
            redis.del("/carstate/car" + std::to_string(i) + "/accel_X");
            redis.del("/carstate/car" + std::to_string(i) + "/accel_Y");
            redis.del("/carstate/car" + std::to_string(i) + "/accel_Z");
        }
        redis.set("/distributed_game_engine", "false");
    }
}
