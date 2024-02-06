#include "SaveManager.h"
#include <fstream>
#include "../util.hpp"
#include "../EngineState.h"

// ************************************* //
// **************  SAVES  ************** //
// ************************************* //

int SaveManager::GetAllLengths() {
    // We need the lengths of all the walls, and anything else we store
    int wallSize = 0;
    for (Wall w : Scene::_walls) {
        wallSize += w.getMemLength();
    }
    wallSize += sizeof(int); /* Add room to tell how big the following walls are */
    int floorSize = 0;
    for (Floor f : Scene::_floors) {
        floorSize += f.getMemLength();
    }
    floorSize += sizeof(int); /* We always reserve the room as if it is 0 we still */
    int trifloorsize = 0;
    for (TriFloor tf : Scene::_trifloors) {
        trifloorsize += tf.getMemLength();
    }
    trifloorsize += sizeof(int); /* Want the file to know that, easier to load. */


    int totalSize = wallSize + floorSize + trifloorsize;
    return totalSize;
}

void SaveManager::SaveScene() {
    if (GetAllLengths() <= 0) return;
    std::byte* File = (std::byte*)malloc(GetAllLengths());
    if (File) {
        /* used for mem length functions */
        Wall dummyWall = Wall();
        Floor dummyFloor = Floor();
        TriFloor dummyTriFloor = TriFloor();
        int offset = 0;

        int amountOfWalls = Scene::_walls.size();
        std::memcpy(File, &amountOfWalls, sizeof(int));
        offset += sizeof(int);
        int amountOfFloors = Scene::_floors.size();
        // The horrid (sizeof(glm::vec3) * 2) * amountOfWalls is to offset the floor count to the end of wall section
        std::memcpy(File + sizeof(int), &amountOfFloors, sizeof(int));
        offset += sizeof(int);
        int amountOfTriFloors = Scene::_trifloors.size();
        std::memcpy(File + sizeof(int) * 2, &amountOfTriFloors, sizeof(int));
        offset += sizeof(int);

        for (Wall w : Scene::_walls) {
            std::byte* wallSerialized = (std::byte*)malloc(w.getMemLength());
            if (!wallSerialized) continue;
            w.serialize(wallSerialized);
            //Its safe trust
            std::memcpy(File + offset, wallSerialized, w.getMemLength());
            free(wallSerialized);
            offset += w.getMemLength();
        }
        for (Floor f : Scene::_floors) {
            std::byte* floorSerialized = (std::byte*)malloc(f.getMemLength());
            if (!floorSerialized) continue;
            f.serialize(floorSerialized);
            std::memcpy(File + offset, floorSerialized, f.getMemLength());
            free(floorSerialized);
            offset += f.getMemLength();
        }
        for (TriFloor tf : Scene::_trifloors) {
            std::byte* trifloorSerialized = (std::byte*)malloc(tf.getMemLength());
            if (!trifloorSerialized) continue;
            tf.serialize(trifloorSerialized);
            std::memcpy(File + offset, trifloorSerialized, tf.getMemLength());
            free(trifloorSerialized);
            offset += tf.getMemLength();
        }

        /* Write to file */
        std::ofstream sceneFile;
        sceneFile.open(filename);

        for (int i = 0; i < GetAllLengths(); ++i)
            sceneFile << (unsigned char)File[i];
        sceneFile.close();
        free(File);
    }
    // Reset all walls now that we have saved them.
    Scene::ClearObjects();
}

int SaveManager::LoadWalls(int loopForOffset, int offset, std::byte* byteArr) {
    int loopFor = 0;
    // Get how many walls there are
    std::memcpy(&loopFor, byteArr + loopForOffset, sizeof(int));
    for (int i = 0; i < loopFor; i++) {
        glm::vec3 wallStart(0);
        glm::vec3 wallEnd(0);
        std::memcpy(&wallStart, byteArr + offset, sizeof(glm::vec3));
        std::memcpy(&wallEnd, byteArr + offset + sizeof(glm::vec3), sizeof(glm::vec3));
        Wall newWall = Wall(wallStart, wallEnd, WALL_HEIGHT, 0);
        offset += newWall.getMemLength();
        Scene::AddWall(newWall);
    }
    return offset;
}

int SaveManager::LoadFloors(int loopForOffset, int offset, std::byte* byteArr) {
    int loopFor = 0;
    // Get how many floor there are
    std::memcpy(&loopFor, byteArr + loopForOffset , sizeof(int));
     for (int i = 0; i < loopFor; i++) {
        glm::vec3 floortopl(0);
        glm::vec3 floorbotr(0);
        float lvls[4];
        std::memcpy(&floortopl, byteArr + offset, sizeof(glm::vec3));
        std::memcpy(&floorbotr, byteArr + offset + sizeof(glm::vec3), sizeof(glm::vec3));
        std::memcpy(&lvls, byteArr + offset + sizeof(glm::vec3) * 2, sizeof(float) * 4);
        Floor floor = Floor(floortopl, floorbotr, lvls, 0);
        offset += floor.getMemLength();
        Scene::AddFloor(floor);
    }
    return offset;
}

int SaveManager::LoadTriFloors(int loopForOffset, int offset, std::byte* byteArr) {
    int loopFor = 0;
    // Get how many floor there are
    std::memcpy(&loopFor, byteArr + loopForOffset, sizeof(int));
    for (int i = 0; i < loopFor; i++) {
        glm::vec3 point1;
        glm::vec3 point2;
        glm::vec3 point3;
        float levels[3];
        std::memcpy(&point1, byteArr + offset,  sizeof(glm::vec3));
        std::memcpy(&point2, byteArr + offset + sizeof(glm::vec3), sizeof(glm::vec3));
        std::memcpy(&point3, byteArr + offset + sizeof(glm::vec3) * 2, sizeof(glm::vec3));
        std::memcpy(&levels, byteArr + offset + sizeof(glm::vec3) * 3, sizeof(float) * 3);
        TriFloor trifloor = TriFloor(point1, point2, point3, levels, 0);
        offset += trifloor.getMemLength();
        Scene::AddTriFloor(trifloor);
    }
    return offset;
}


void SaveManager::LoadScene() { LoadScene(filename); } /* If you don't pass in a file, we are going to use the player input */

// Over load of LoadScene where we pass in the file instead of taking it from the UI, used when loading into a set scene (See Scene::Load())
void SaveManager::LoadScene(std::string file) {
    std::ifstream sceneFile(file, std::ifstream::binary);
    if (!sceneFile) return;
    Scene::ClearObjects();
    sceneFile.seekg(0, sceneFile.end);
    std::streamsize len = sceneFile.tellg();
    sceneFile.seekg(0, sceneFile.beg);
    std::byte* File = (std::byte*)malloc(len);
    sceneFile.read((char*)File, len);
    if (File) {
        int offset = sizeof(int) * 3;
        offset = LoadWalls(0, offset, File);
        offset = LoadFloors(sizeof(int), offset, File);
        offset = LoadTriFloors(sizeof(int) * 2, offset, File);
        // Done loading all objects
        free(File);
    }
}