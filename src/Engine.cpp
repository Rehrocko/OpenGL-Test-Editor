#include "Common.h"
#include "util.hpp"
#include "Engine.h"
#include "Renderer/renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* window;

void Engine::Run() {
    Init();

    double oldTime = glfwGetTime();
    double currentTime = oldTime;
    double dTAccumulator = 0.0;
    double fixedDeltaTime = 1.0 / 60.0; // 60.0 being the TPS for physics wanted

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Time stuff
        oldTime = currentTime;
        currentTime = glfwGetTime();
        double deltaTime = currentTime - oldTime;
        dTAccumulator += deltaTime;

        while (dTAccumulator >= fixedDeltaTime) {
            dTAccumulator -= fixedDeltaTime;
            // Physics runs here
        }
        Renderer::RenderFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return;
}

void Engine::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scene test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //glfwSetKeyCallback(window, KeyPress);

    Renderer::Init();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}