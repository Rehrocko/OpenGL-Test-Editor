#include "Renderer.h"
#include "shader.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
using namespace Renderer;

struct Shaders {
	Shader geometry;
} _shaders;

bool _depthOfFieldScene = 0.9f;

glm::vec3 sceneColor;
float col[3];

void DrawScene(Shader& shader);
void GeometryPass(Player* player);

void Renderer::Init() {
    

    _shaders.geometry.Load("geometry.vert", "geometry.frag");
}

void Renderer::RenderFrame(Player* player) {
    GeometryPass(player);
}

void Renderer::ReloadShaders() {
    std::cout << "Reloading shaders";

    _shaders.geometry.Load("geometry.vert", "geometry.frag");
}

void GeometryPass(Player* player) {
    glm::mat4 projection = Renderer::GetProjectionMatrix(_depthOfFieldScene);
    glm::mat4 view = player->GetViewMatrix();

    glViewport(0, 0, GL::GetWindowWidth(), GL::GetWindowHeight());
    glEnable(GL_DEPTH_TEST);
    _shaders.geometry.Use();
    _shaders.geometry.SetMat4("projection", projection);
    _shaders.geometry.SetMat4("view", view);
    _shaders.geometry.SetVec3("viewPos", player->GetViewPos());
    _shaders.geometry.SetVec3("camForward", player->GetCameraForward());
    _shaders.geometry.SetBool("isPlayer", false);
    DrawScene(_shaders.geometry);
}
    
void DrawScene(Shader& shader) {
    shader.SetMat4("model", glm::mat4(1));
    for (Wall& wall : Scene::_walls) {
        wall.Draw();
    }

    for (Floor& floor : Scene::_floors) {
        floor.Draw();
    }

    for (TriFloor& trifloor : Scene::_trifloors) {
        trifloor.Draw();
    }

    //Test cube of player
    // Quick vertices of a cube
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f,
    };
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Move the cube
    glm::mat4 cube = glm::mat4(1);
    cube = glm::translate(cube, Scene::_players[0].GetPos());
    shader.SetMat4("model", cube);
    shader.SetBool("isPlayer", true);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::mat4 Renderer::GetProjectionMatrix(float depthOfField) {
    float width = (float)GL::GetWindowWidth();
    float height = (float)GL::GetWindowHeight();

    return glm::perspective(depthOfField, width / height, NEAR_PLANE, FAR_PLANE);
}