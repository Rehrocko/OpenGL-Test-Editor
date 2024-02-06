#pragma once
#include "../Common.h"
#include "Player.h"
#include <vector>

#define WALL_HEIGHT 3.0f
#define FLOOR_LEVEL -0.5f

inline glm::vec3 NormalFromThreePoints(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2) {
    return glm::normalize(glm::cross(pos1 - pos0, pos2 - pos0));
}

inline void SetNormalsAndTangentsFromVertices(Vertex* vert0, Vertex* vert1, Vertex* vert2) {
    // Shortcuts for UVs
    glm::vec3& v0 = vert0->position;
    glm::vec3& v1 = vert1->position;
    glm::vec3& v2 = vert2->position;
    glm::vec2& uv0 = vert0->uv;
    glm::vec2& uv1 = vert1->uv;
    glm::vec2& uv2 = vert2->uv;
    // Edges of the triangle : position delta. UV delta
    glm::vec3 deltaPos1 = v1 - v0;
    glm::vec3 deltaPos2 = v2 - v0;
    glm::vec2 deltaUV1 = uv1 - uv0;
    glm::vec2 deltaUV2 = uv2 - uv0;
    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
    glm::vec3 normal = NormalFromThreePoints(vert0->position, vert1->position, vert2->position);
    vert0->normal = normal;
    vert1->normal = normal;
    vert2->normal = normal;
    vert0->tangent = tangent;
    vert1->tangent = tangent;
    vert2->tangent = tangent;
    vert0->bitangent = bitangent;
    vert1->bitangent = bitangent;
    vert2->bitangent = bitangent;
}

struct Wall {
	glm::vec3 begin = glm::vec3(0);
    glm::vec3 end;
    float height = 0;
    GLuint VAO = 0;
    GLuint VBO = 0;
    std::vector<Vertex> vertices;
    int materialIndex = 0;
    float wallHeight = 2.4f;
    void CreateMesh();
    Wall(glm::vec3 begin, glm::vec3 end, float height, int materialIndex);
    Wall();
    glm::vec3 GetNormal();
    glm::vec3 GetMidPoint();
    void Draw();
    // Saving stuff
    std::byte* serialize(std::byte* out);
    void deserialize(std::byte* in);
    int getMemLength();
};

struct Floor {
    glm::vec3 topLeft;
    glm::vec3 bottomRight;
    float level[4];
    GLuint VAO = 0;
    GLuint VBO = 0;
    std::vector<Vertex> vertices;
    int materialIndex = 0;
    void CreateMesh();
    Floor(glm::vec3 topLeft, glm::vec3 bottomRight, float level[4], int materialIndex);
    Floor();
    void Draw();
    // Saving stuff
    std::byte* serialize(std::byte* out);
    void deserialize(std::byte* in);
    int getMemLength();
};

struct TriFloor {
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    float levels[3];
    GLuint VAO = 0;
    GLuint VBO = 0;
    std::vector<Vertex> vertices;
    int materialIndex = 0;
    void CreateMesh();
    TriFloor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float level[3], int materialIndex);
    TriFloor();
    void Draw();
    // Saving stuff
    std::byte* serialize(std::byte* out);
    void deserialize(std::byte* in);
    int getMemLength();
};

namespace Scene {
    //std::vector<GameObject> _gameObjects;
    void CleanUp();
    void ClearObjects();
    void Update(float deltatime);
    void Load();
    void LoadPlayers();
    inline std::vector<Player> _players;

    void AddWall(Wall& wall);
    inline std::vector<Wall> _walls;
    void AddFloor(Floor& floor);
    inline std::vector<Floor> _floors;
    void AddTriFloor(TriFloor& trifloor);
    inline std::vector<TriFloor> _trifloors;
}