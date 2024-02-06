#include "Scene.h"
#include "SaveManager.h"

// ************************************* //
// ************** SCENERY ************** //
// ************************************* //
void Scene::Update(float deltaTime) {
    for (Player& plr : _players) {
        plr.Update(deltaTime);
    }
}

void Scene::LoadPlayers() {
    // Main player
    _players.push_back(Player(glm::vec3(2.5f, 0.5f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

//Has the same functionality as ClearObjects but I forgot about it and am too lazy to delete, may eventually be used for something else
void Scene::CleanUp() {
	_walls.clear();
    _floors.clear();
}

void Scene::ClearObjects() {
    _walls.clear();
    _floors.clear();
    _trifloors.clear();
}

void Scene::AddWall(Wall& wall) {
	_walls.push_back(wall);
}

void Scene::AddFloor(Floor& floor) {
    _floors.push_back(floor);
}

void Scene::AddTriFloor(TriFloor& trifloor) {
    _trifloors.push_back(trifloor);
}

// Load the main scene
void Scene::Load() {
    SaveManager::LoadScene("map.dat");
}

// ************************************* //
// **************  WALLS  ************** //
// ************************************* //
Wall::Wall(glm::vec3 begin, glm::vec3 end, float height, int materialIndex) {
    this->materialIndex = materialIndex;
    this->begin = begin;
    this->end = end;
    this->height = height;
    CreateMesh();
}

Wall::Wall() {

}

void Wall::CreateMesh() {
    vertices.clear();

    //Init
    bool finishedWall = false;
    glm::vec3 wallStart = begin;
    glm::vec3 wallEnd = end;
    glm::vec3 cursor = wallStart;
    glm::vec3 wallDir = glm::normalize(wallEnd - cursor);
    float texScale = 2.0f;

    float uvX1 = 0;
    float uvX2 = 0;
    
    int count = 0;
    while (!finishedWall || count > 1000) {
        count++;
        Vertex v1, v2, v3, v4;
        v1.position = cursor;
        v2.position = cursor + glm::vec3(0, height, 0);
        v3.position = wallEnd + glm::vec3(0, height, 0);
        v4.position = wallEnd;
        float segmentWidth = abs(glm::length((v4.position - v1.position))) / WALL_HEIGHT;
        float segmentHeight = glm::length((v2.position - v1.position)) / WALL_HEIGHT;
        uvX2 = uvX1 + segmentWidth;
        v1.uv = glm::vec2(uvX1, segmentHeight) * texScale;
        v2.uv = glm::vec2(uvX1, 0) * texScale;
        v3.uv = glm::vec2(uvX2, 0) * texScale;
        v4.uv = glm::vec2(uvX2, segmentHeight) * texScale;

        SetNormalsAndTangentsFromVertices(&v3, &v2, &v1);
        SetNormalsAndTangentsFromVertices(&v3, &v1, &v4);
        vertices.push_back(v3);
        vertices.push_back(v2);
        vertices.push_back(v1);
        vertices.push_back(v3);
        vertices.push_back(v1);
        vertices.push_back(v4);
        finishedWall = true;
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
}

glm::vec3 Wall::GetNormal() {
    glm::vec3 vector = glm::normalize(begin - end);
    return glm::vec3(-vector.z, 0, vector.x) * glm::vec3(-1);
}

glm::vec3 Wall::GetMidPoint() {
    return (begin + end) * glm::vec3(0.5);
}

void Wall::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

std::byte* Wall::serialize(std::byte* out) {
    std::memcpy(out, &begin, sizeof(glm::vec3));
    std::memcpy(out + sizeof(glm::vec3), &end, sizeof(glm::vec3));
    return out;
}

void Wall::deserialize(std::byte* in) {
    std::memcpy(&begin, in, sizeof(glm::vec3));
    std::memcpy(&end, in + sizeof(glm::vec3), sizeof(glm::vec3));
}

int Wall::getMemLength() {
    // Our save length for this object is exactly this size.
    return sizeof(glm::vec3) * 2;
}

// ************************************* //
// **************  FLOOR  ************** //
// ************************************* //

Floor::Floor(glm::vec3 topLeft, glm::vec3 bottomRight, float level[4], int materialIndex) {
    this->materialIndex = materialIndex;
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
    //this->level = level[4];
    std::copy(level, level + 4, this->level);
    CreateMesh();
}

Floor::Floor() {

}

void Floor::CreateMesh() {
    vertices.clear();

    //Init
    bool finishedFloor = false;
    glm::vec3 floorStart = topLeft;
    glm::vec3 floorEnd = bottomRight;
    float texScale = 2.0f;

    int count = 0;
    while (!finishedFloor || count > 1000) {
        count++;
        Vertex v1, v2, v3, v4;
        v1.position = floorStart + glm::vec3(0, level[0], 0);            /* Top left     */
        v2.position = glm::vec3(floorStart.x,   level[1], floorEnd.z);   /* Top Right    */
        v3.position = floorEnd + glm::vec3(0,   level[2], 0);            /* Bottom Right */
        v4.position = glm::vec3(floorEnd.x,     level[3], floorStart.z); /* Bottom left  */

        vertices.push_back(v3);
        vertices.push_back(v2);
        vertices.push_back(v1);
        vertices.push_back(v3);
        vertices.push_back(v1);
        vertices.push_back(v4);
        finishedFloor = true;
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, ));
    //glEnableVertexAttribArray(1);
}

void Floor::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

std::byte* Floor::serialize(std::byte* out) {
    std::memcpy(out, &topLeft, sizeof(glm::vec3));
    std::memcpy(out + sizeof(glm::vec3), &bottomRight, sizeof(glm::vec3));
    std::memcpy(out + sizeof(glm::vec3) * 2, &level, sizeof(float) * 4);
    return out;
}

void Floor::deserialize(std::byte* in) {
    std::memcpy(&topLeft, in, sizeof(glm::vec3));
    std::memcpy(&bottomRight, in + sizeof(glm::vec3), sizeof(glm::vec3));
    std::memcpy(&level, in + sizeof(glm::vec3) * 2, sizeof(float) * 4);
}

int Floor::getMemLength() {
    // Our save length for this object is exactly this size.
    return (sizeof(glm::vec3) * 2) + sizeof(float) * 4;
}

// **************************************** //
// **************  TRIFLOOR  ************** //
// **************************************** //

TriFloor::TriFloor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float level[3], int materialIndex) {
    this->materialIndex = materialIndex;
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    std::copy(level, level + 3, this->levels);
    CreateMesh();
}

TriFloor::TriFloor() {

}

void TriFloor::CreateMesh() {
    vertices.clear();

    //Init
    bool finishedFloor = false;
    float texScale = 2.0f;

    int count = 0;
    while (!finishedFloor || count > 1000) {
        count++;
        Vertex v1, v2, v3;
        v1.position = p1 + glm::vec3(0, levels[0], 0);
        v2.position = p2 + glm::vec3(0, levels[1], 0);
        v3.position = p3 + glm::vec3(0, levels[2], 0);

        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        finishedFloor = true;
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, ));
    //glEnableVertexAttribArray(1);
}

void TriFloor::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

std::byte* TriFloor::serialize(std::byte* out) {
    std::memcpy(out,                         &p1,     sizeof(glm::vec3));
    std::memcpy(out + sizeof(glm::vec3),     &p2,     sizeof(glm::vec3));
    std::memcpy(out + sizeof(glm::vec3) * 2, &p3,     sizeof(glm::vec3));
    std::memcpy(out + sizeof(glm::vec3) * 3, &levels, sizeof(float) * 3);
    return out;
}

void TriFloor::deserialize(std::byte* in) {
    std::memcpy(&p1,      in,  sizeof(glm::vec3));
    std::memcpy(&p2,      in + sizeof(glm::vec3),     sizeof(glm::vec3));
    std::memcpy(&p3,      in + sizeof(glm::vec3) * 2, sizeof(glm::vec3));
    std::memcpy(&levels,  in + sizeof(glm::vec3) * 3, sizeof(float) * 3);
}

int TriFloor::getMemLength() {
    // Our save length for this object is exactly this size.
    return (sizeof(glm::vec3) * 3) + sizeof(float) * 3;
}