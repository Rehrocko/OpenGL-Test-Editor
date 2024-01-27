#include "Renderer.h"
#include "shader.h"

struct Shaders {
	Shader solids;
} shaders;

unsigned int VAO, VBO;
bool wireframe = false;

void Renderer::Init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    shaders.solids.Load("test.vert", "test.frag");
    float vertices[] = {
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Renderer::RenderFrame() {
    shaders.solids.Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::ReloadShaders() {
    std::cout << "Reloading shaders";

    shaders.solids.Load("test.vert", "test.frag");
}

void Renderer::ToggleWireframe() {
    if (!wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframe = true;
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireframe = false;
    }
}