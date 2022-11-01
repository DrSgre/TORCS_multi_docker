#include <etcd/Client.hpp>

#include <chrono>
#include <thread>
#include <raceman.h>

#define SNAPSHOT_FREQUENCY 100

extern etcd::Client etcd_client;

void SaveState(tSituation *s)
{
    // If the ETCD values are different from the local ones, they are updated accordingly.
    if (std::stod(etcd_client.get("/state/deltaTime").get().value().as_string()) != s->deltaTime) {
        etcd_client.set("/state/deltaTime", std::to_string(s->deltaTime));
    }
    if (std::stod(etcd_client.get("/state/currentTime").get().value().as_string()) != s->currentTime) {
        etcd_client.set("/state/currentTime", std::to_string(s->currentTime));
    }
    if (std::stoi(etcd_client.get("/state/raceState").get().value().as_string()) != s->_raceState) {
        etcd_client.set("/state/raceState", std::to_string(s->_raceState));
    }
    for (int i = 0; i < s->_ncars; i++) {
        // Store the information related to the position with respect to the track segment.
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toStart").get().value().as_string()) != s->cars[i]->_trkPos.toStart) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toStart", std::to_string(s->cars[i]->_trkPos.toStart));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toMiddle").get().value().as_string()) != s->cars[i]->_trkPos.toMiddle) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toMiddle", std::to_string(s->cars[i]->_trkPos.toMiddle));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toRight").get().value().as_string()) != s->cars[i]->_trkPos.toRight) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toRight", std::to_string(s->cars[i]->_trkPos.toRight));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toLeft").get().value().as_string()) != s->cars[i]->_trkPos.toLeft) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toLeft", std::to_string(s->cars[i]->_trkPos.toLeft));
        }
        if (std::stoi(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_type").get().value().as_string()) != s->cars[i]->_trkPos.type) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_type", std::to_string(s->cars[i]->_trkPos.type));
        }
        if (std::stoi(etcd_client.get("/carstate/car" + std::to_string(i) + "/seg_id").get().value().as_string()) != s->cars[i]->_trkPos.seg->id) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/seg_id", std::to_string(s->cars[i]->_trkPos.seg->id));
        }

        // Store the information related to the global position of the car.
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_X").get().value().as_string()) != s->cars[i]->_pos_X) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_X", std::to_string(s->cars[i]->_pos_X));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_Y").get().value().as_string()) != s->cars[i]->_pos_Y) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_Y", std::to_string(s->cars[i]->_pos_Y));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_Z").get().value().as_string()) != s->cars[i]->_pos_Z) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_Z", std::to_string(s->cars[i]->_pos_Z));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_AX").get().value().as_string()) != s->cars[i]->_roll) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_AX", std::to_string(s->cars[i]->_roll));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_AY").get().value().as_string()) != s->cars[i]->_pitch) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_AY", std::to_string(s->cars[i]->_pitch));
        }
        if (std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_AZ").get().value().as_string()) != s->cars[i]->_yaw) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_AZ", std::to_string(s->cars[i]->_yaw));
        }
    }
}

void LoadState (tRmInfo *ReInfo) {
    tSituation *s = ReInfo->s;
    // If the local values are different from the ones in ETCD, they are updated accordingly.
    double deltaTime = std::stod(etcd_client.get("/state/deltaTime").get().value().as_string());
    if (deltaTime != s->deltaTime) {
        s->deltaTime = deltaTime;
    }
    double currentTime = std::stod(etcd_client.get("/state/currentTime").get().value().as_string());
    if (currentTime != s->currentTime) {
        s->currentTime = currentTime;
    }
    int raceState = std::stoi(etcd_client.get("/state/raceState").get().value().as_string());
    if (raceState != s->_raceState) {
        s->_raceState = raceState;
    }

    // For each car in the race, get the data related to the position with respect to the track segment and the global position of the car.
    for (int i = 0; i < s->_ncars; i++) {
        double pos_toStart = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toStart").get().value().as_string());
        if (pos_toStart != s->cars[i]->_trkPos.toStart) {
            s->cars[i]->_trkPos.toStart = pos_toStart;
        }
        double pos_toMiddle = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toMiddle").get().value().as_string());
        if (pos_toMiddle != s->cars[i]->_trkPos.toMiddle) {
            s->cars[i]->_trkPos.toMiddle = pos_toMiddle;
        }
        double pos_toRight = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toRight").get().value().as_string());
        if (pos_toRight != s->cars[i]->_trkPos.toRight) {
            s->cars[i]->_trkPos.toRight  = pos_toRight;
        }
        double pos_toLeft = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_toLeft").get().value().as_string());
        if (pos_toLeft != s->cars[i]->_trkPos.toLeft) {
            s->cars[i]->_trkPos.toLeft = pos_toLeft;
        }
        int pos_type = std::stoi(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_type").get().value().as_string());
        if (pos_type != s->cars[i]->_trkPos.type) {
            s->cars[i]->_trkPos.type = pos_type;
        }
        // In order to get the correct track segment, without storing needless data, we use the segment id as reference for iterating
        // on the list of track segments.
        int seg_id = std::stoi(etcd_client.get("/carstate/car" + std::to_string(i) + "/seg_id").get().value().as_string());
        while (s->cars[i]->_trkPos.seg->id != seg_id) {
            if (s->cars[i]->_trkPos.seg->id > seg_id) {
                s->cars[i]->_trkPos.seg = s->cars[i]->_trkPos.seg->prev;
            }
            else {
                s->cars[i]->_trkPos.seg = s->cars[i]->_trkPos.seg->next;
            }
        }

        double pos_X = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_X").get().value().as_string());
        if (pos_X != s->cars[i]->_pos_X) {
            s->cars[i]->_pos_X = pos_X;
        }
        double pos_Y = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_Y").get().value().as_string());
        if (pos_Y != s->cars[i]->_pos_Y) {
            s->cars[i]->_pos_Y = pos_Y;
        }
        double pos_Z = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_Z").get().value().as_string());
        if (pos_Z != s->cars[i]->_pos_Z) {
            s->cars[i]->_pos_Z  = pos_Z;
        }
        double pos_AX = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_AX").get().value().as_string());
        if (pos_AX != s->cars[i]->_roll) {
            s->cars[i]->_roll = pos_AX;
        }
        double pos_AY = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_AY").get().value().as_string());
        if (pos_AY != s->cars[i]->_pitch) {
            s->cars[i]->_pitch = pos_AY;
        }
        double pos_AZ = std::stod(etcd_client.get("/carstate/car" + std::to_string(i) + "/pos_AZ").get().value().as_string());
        if (pos_AZ != s->cars[i]->_yaw) {
            s->cars[i]->_yaw  = pos_AZ;
        }

        ReInfo->_reSimItf.config(s->cars[i], ReInfo);
    }
}

void StartStateManager(tRmInfo* ReInfo)
{
    tSituation *s = ReInfo->s;
    // If a race is running and values are already set in ETCD, then the local GE gets them.
    // There is no need to get the time/racestate related values, as they are loaded during the loop.
    if (etcd_client.get("/state/currentTime").get().is_ok()) {
        s->_raceType = std::stoi(etcd_client.get("/state/raceType").get().value().as_string());
        s->_totLaps = std::stoi(etcd_client.get("/state/totLaps").get().value().as_string());
        s->_ncars = std::stoi(etcd_client.get("/state/ncars").get().value().as_string());
        s->_maxDammage = std::stoi(etcd_client.get("/state/maxDammage").get().value().as_string());
    }
    // Otherwise, the values are set in ETCD just once, at the beginning of the race.
    else {
        etcd_client.set("/state/raceType", std::to_string(s->_raceType));
        etcd_client.set("/state/totLaps", std::to_string(s->_totLaps));
        etcd_client.set("/state/ncars", std::to_string(s->_ncars));
        etcd_client.set("/state/maxDammage", std::to_string(s->_maxDammage));

        // Initialize game state values set multiple times, to allow for later comparisons.
        etcd_client.set("/state/deltaTime", std::to_string(s->deltaTime));
        etcd_client.set("/state/currentTime", std::to_string(s->currentTime));
        etcd_client.set("/state/raceState", std::to_string(s->_raceState));

        // Initialize car state values, to allow for later comparisons.
        for (int i = 0; i < s->_ncars; i++) {
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toStart", std::to_string(s->cars[i]->_trkPos.toStart));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toMiddle", std::to_string(s->cars[i]->_trkPos.toMiddle));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toRight", std::to_string(s->cars[i]->_trkPos.toRight));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_toLeft", std::to_string(s->cars[i]->_trkPos.toLeft));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_type", std::to_string(s->cars[i]->_trkPos.type));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/seg_id", std::to_string(s->cars[i]->_trkPos.seg->id));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_X", std::to_string(s->cars[i]->_pos_X));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_Y", std::to_string(s->cars[i]->_pos_Y));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_Z", std::to_string(s->cars[i]->_pos_Z));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_AX", std::to_string(s->cars[i]->_roll));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_AY", std::to_string(s->cars[i]->_pitch));
            etcd_client.set("/carstate/car" + std::to_string(i) + "/pos_AZ", std::to_string(s->cars[i]->_yaw));
        }
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
            // If the race time on ETCD is greater than the one in local, this means that the local state is obsolete 
            // and needs update. Otherwise, the state is updated and the GE proceeds with saving it in ETCD.
            if (s->currentTime < std::stod(etcd_client.get("/state/currentTime").get().value().as_string()))
            {
                LoadState(ReInfo);
            }
            else 
            {
                SaveState(ReInfo->s);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(SNAPSHOT_FREQUENCY));
        }
    }
    // Clean up ETCD keys, before stopping the State Manager.
    etcd_client.rm("/state/raceType");
    etcd_client.rm("/state/totLaps");
    etcd_client.rm("/state/ncars");
    etcd_client.rm("/state/maxDammage");
    etcd_client.rm("/state/deltaTime");
    etcd_client.rm("/state/currentTime");
    etcd_client.rm("/state/raceState");
    for (int i = 0; i < s->_ncars; i++) {
        etcd_client.rm("/carstate/car" + std::to_string(i) + "/pos_toStart");
        etcd_client.rm("/carstate/car" + std::to_string(i) + "/pos_toMiddle");
        etcd_client.rm("/carstate/car" + std::to_string(i) + "/pos_toRight");
        etcd_client.rm("/carstate/car" + std::to_string(i) + "/pos_toLeft");
        etcd_client.rm("/carstate/car" + std::to_string(i) + "/pos_type");
        etcd_client.rm("/carstate/car" + std::to_string(i) + "/seg_id");
    }
}

