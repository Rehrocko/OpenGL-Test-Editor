#include "Common.h"
#include "util.hpp"
#include "Engine.h"
#include "Renderer/Renderer.h"
#include "Core/GL.h"
#include "Core/Input.h"
#include "Core/Player.h"
#include "Core/Scene.h"
#include "Core/UI.h"
#include "EngineState.h"

GLFWwindow* window;

void Engine::Run() {

    // WIndow Initialize
    GL::Init(1920, 1000);

    // CHANGE THIS TO TRUE WHEN FINISH ASSET MANAGER
    // CHANGE THIS TO TRUE WHEN FINISH ASSET MANAGER
    // CHANGE THIS TO TRUE WHEN FINISH ASSET MANAGER
    // CHANGE THIS TO TRUE WHEN FINISH ASSET MANAGER
    bool assetManagerDone = false;

    while (GL::WindowIsOpen() && assetManagerDone == true) {

        if (Input::KeyPressed(GLFW_KEY_F)) {
            GL::ToggleFullscreen();
        }
        if (Input::KeyPressed(GLFW_KEY_ESCAPE)) {
            return;
        }
        GL::ProcessInput();
        Input::Update();
    }

    Init();

    double lastFrame = glfwGetTime();
    double thisFrame = lastFrame;
    double deltaTimeAccumulator = 0.0;
    double fixedDeltaTime = 1.0 / 60.0;

    while (GL::WindowIsOpen() && GL::WindowHasNotBeenForceClosed()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lastFrame = thisFrame;
        thisFrame = glfwGetTime();
        double deltaTime = thisFrame - lastFrame;
        deltaTimeAccumulator += deltaTime;

        GL::ProcessInput();

        // ************************************* //
        // ************** UPDATES ************** //
        // ************************************* //
        UIHandler::Update(deltaTime);
        Input::Update();
        Scene::Update(deltaTime);
        KeyPresses();

        // ************************************* //
        // ************** RENDERS ************** //
        // ************************************* //
        Renderer::RenderFrame(&Scene::_players[0]);
        UIHandler::Render();

        GL::SwapBuffersPollEvents();
    }

    GL::Terminate();
    return;
}

void Engine::Init() {
    Input::Init();
    Scene::Load();
    Scene::LoadPlayers();

    Renderer::Init();
}

void Engine::KeyPresses() {
    if (Input::KeyPressed(GLFW_KEY_F)) {
        GL::ToggleFullscreen();
    }
    if (Input::KeyPressed(GLFW_KEY_Z)) {
        GL::ToggleCursor();
    }
    if (EngineState::engineState == "EDITOR") {
        if (Input::KeyPressed(GLFW_KEY_KP_ADD)) {
            UIHandler::gridSize += 5;
            UIHandler::floorPlanSpeed -= 0.01f;
            if (UIHandler::floorPlanSpeed < 0.05f) UIHandler::floorPlanSpeed = 0.05f;
            UIHandler::HideCursor();
        }
        if (Input::KeyPressed(GLFW_KEY_KP_SUBTRACT)) {
            UIHandler::gridSize -= 5;
            UIHandler::floorPlanSpeed += 0.01f;
            if (UIHandler::floorPlanSpeed > 0.15f) UIHandler::floorPlanSpeed = 0.15f;
            if (UIHandler::gridSize < 5) {
                UIHandler::gridSize = 5;
            }
            UIHandler::HideCursor();
        }
        if (Input::KeyDown(GLFW_KEY_W)) {
            UIHandler::offset.z -= UIHandler::floorPlanSpeed;
            UIHandler::HideCursor();
        }
        if (Input::KeyDown(GLFW_KEY_S)) {
            UIHandler::offset.z += UIHandler::floorPlanSpeed;
            UIHandler::HideCursor();
        }
        if (Input::KeyDown(GLFW_KEY_A)) {
            UIHandler::offset.x -= UIHandler::floorPlanSpeed;
            UIHandler::HideCursor();
        }
        if (Input::KeyDown(GLFW_KEY_D)) {
            UIHandler::offset.x += UIHandler::floorPlanSpeed;
            UIHandler::HideCursor();
        }
        if (Input::KeyPressed(GLFW_KEY_N)) {
            if (UIHandler::mode == "WALL") UIHandler::CreateWall();
            if (UIHandler::mode == "FLOOR") UIHandler::CreateFloor();
            if (UIHandler::mode == "TRIFLOOR") UIHandler::CreateTriFloor();
            UIHandler::HideCursor();
        }
        if (Input::KeyPressed(GLFW_KEY_C) && UIHandler::mode == "TRIFLOOR")
            UIHandler::cycle = !UIHandler::cycle;
    }
}