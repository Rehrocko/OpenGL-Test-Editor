#include "UI.h"
#include "../util.hpp"
#include "../EngineState.h"

void UIHandler::Update(float deltaTime) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

    // ************************************* //
    // ************** EDITING ************** //
    // ************************************* //
    Editor();
    
}

void UIHandler::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Additional Functions

void UIHandler::HideCursor() {
    startMouse = endMouse = ImVec2(-gridSize, -gridSize);
}

void UIHandler::CreateWall() {
    Wall WallObject = Wall(glm::make_vec3(wallStart), glm::make_vec3(wallEnd), WALL_HEIGHT, 0);
    Scene::AddWall(WallObject);
}

void UIHandler::CreateFloor() {
    Floor FloorObject = Floor(floorTL, floorBR, levels, 0);
    Scene::AddFloor(FloorObject);
}

void UIHandler::CreateTriFloor() {
    TriFloor TriFloorObject = TriFloor(glm::vec3(p1.x, 0, p1.y),
                            (cycle) ?  glm::vec3(p2OptionA.x, 0, p2OptionA.y) : glm::vec3(p2OptionB.x, 0, p2OptionB.y),
                                       glm::vec3(p3.x, 0, p3.y), tlevels, 0);
    Scene::AddTriFloor(TriFloorObject);
}

void UIHandler::Editor() {
    bool open = true;
    //ImGui::ShowStackToolWindow();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(300, GL::GetWindowHeight()));
    ImGui::Begin("Scene Editor", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    SceneButtons();
    if (ImGui::CollapsingHeader("Walls")) {
        ImGui::InputFloat3("Wall Start position", glm::value_ptr(wallStart));
        ImGui::InputFloat3("Wall End position", glm::value_ptr(wallEnd));
        if (ImGui::BeginListBox("Walls", ImVec2(-1, -1))) {
            if (ImGui::Button("New Wall (N)") && mode == "WALL")
                CreateWall();
            int itr = 0;
            for (Wall w : Scene::_walls) {
                ImGui::PushID(itr);
                ImGui::Text("--------");
                ImGui::InputFloat3("", glm::value_ptr(w.begin), "%.1f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("", glm::value_ptr(w.end), "%.1f", ImGuiInputTextFlags_ReadOnly);
                if (ImGui::Button("Remove")) {
                    Scene::_walls.erase(Scene::_walls.begin() + itr);
                }
                itr += 1;
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
    }
    if (ImGui::CollapsingHeader("Floors")) {
        ImGui::InputFloat3("Floor Top Left", glm::value_ptr(floorTL));
        ImGui::InputFloat3("Floor Bottom Right", glm::value_ptr(floorBR));
        float levelsBuf[4];
        std::copy(levels, levels + 4, levelsBuf);
        ImGui::InputFloat4("Floor Levels", levelsBuf);
        std::copy(levelsBuf, levelsBuf + 4, levels);
        if (ImGui::BeginListBox("Floors", ImVec2(-1, -1))) {
            if (ImGui::Button("New Floor (N)") && mode == "FLOOR")
                CreateFloor();
            int itr = 0;
            for (Floor f : Scene::_floors) {
                ImGui::PushID(itr);
                ImGui::Text("--------");
                ImGui::InputFloat3("", glm::value_ptr(f.topLeft), "%.1f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("", glm::value_ptr(f.bottomRight), "%.1f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat4("", levels, "%.1f", ImGuiInputTextFlags_ReadOnly);
                if (ImGui::Button("Remove")) {
                    Scene::_floors.erase(Scene::_floors.begin() + itr);
                }
                itr += 1;
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
    }
    if (ImGui::CollapsingHeader("Tri-floors")) {
        // Turn the ImVec2 into float2s
        float point1[2] = { p1.x, p1.y };
        float point2OptionA[2] = {p2OptionA.x, p2OptionA.y};
        float point2OptionB[2] = {p2OptionB.x, p2OptionB.y};
        float point3[2] = { p3.x, p3.y };
        ImGui::InputFloat2("Point 1", point1);
        if (cycle) ImGui::InputFloat2("Point 2", point2OptionA);
        else       ImGui::InputFloat2("Point 2", point2OptionB);
        ImGui::InputFloat2("Point 3", point3);
        float levelBuf[3];
        std::copy(tlevels, tlevels + 3, levelBuf);
        ImGui::InputFloat3("Levels", levelBuf);
        std::copy(levelBuf, levelBuf + 3, tlevels);
        if (ImGui::BeginListBox("Tri-floors", ImVec2(-1, -1))) {
            if (ImGui::Button("New Tri-floor (N)") && mode == "TRIFLOOR")
                CreateTriFloor();
            int itr = 0;
            for (TriFloor tf : Scene::_trifloors) {
                ImGui::PushID(itr);
                ImGui::Text("--------");
                ImGui::InputFloat3("", glm::value_ptr(tf.p1), "%.1f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("", glm::value_ptr(tf.p2), "%.1f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("", glm::value_ptr(tf.p3), "%.1f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("", tf.levels, "%.1f", ImGuiInputTextFlags_ReadOnly);
                if (ImGui::Button("Remove")) {
                    Scene::_trifloors.erase(Scene::_trifloors.begin() + itr);
                }
                itr += 1;
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
    }
    ImGui::End();
}

void UIHandler::SceneButtons() {
    if (ImGui::Button("Open Floorplan")) {
        _openFloorPlan = !_openFloorPlan;
        if (_openFloorPlan && ImGui::IsItemVisible())
            EngineState::engineState = "EDITOR";
        else
            EngineState::engineState = "PLAYER";
    }
    if (ImGui::Button("Clear Scene"))
        Scene::ClearObjects();
    if (ImGui::Button("Save Scene"))
        SaveManager::SaveScene();
    ImGui::SameLine();
    if (ImGui::Button("Load Scene"))
        SaveManager::LoadScene();
    // Filename Input
    bool pushingStyle = false;
    if (SaveManager::filename.length() > 126) {
        ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(255, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, (ImVec4)ImColor(255, 0, 0));
        //ImGui::PushStyleColor(imguicol_input, (ImVec4)ImColor(255, 0, 0));
        pushingStyle = true;
    }
    if (SaveManager::filename == "")
        SaveManager::filename = "scene.dat";
    char buf[128] = "";
    strcpy_s(buf, sizeof(buf), SaveManager::filename.c_str());
    ImGui::Text("Filename");
    ImGui::InputText("##inputfield", buf, sizeof(buf));
    ImGui::Text(SaveManager::filename.c_str());
    if (pushingStyle) {
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    SaveManager::filename = buf;
    // End
    if (_openFloorPlan) {
        Floorplan();
        SwapModeButtons();
    }
    else
        HideCursor();
}

void UIHandler::Floorplan() {
    bool open = true;
    ImGui::SetNextWindowSize(ImVec2(GL::GetWindowWidth() - ImGui::GetWindowWidth(), GL::GetWindowHeight()));
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowWidth(), 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    // This is a bit hard to pick apart, so if you are me in the future, you can either re write this or pray that you remember, because I am not commenting this.
    if (ImGui::Begin("##pospicker", &open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
        //ImGui::ShowStackToolWindow();
        // The background button (changing colors so that it just seems like a plain background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2588f, 0.5882f, 0.9803f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2588f, 0.5882f, 0.9803f, 0.4f));
        ImGui::Button("##button", ImVec2(-1, -1));
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        //Gui::SetItemAllowOverlap();
        bool isHovered = ImGui::IsItemHovered();
        ImVec2 mPos = ImGui::GetMousePos();
        screenPos = ImGui::GetItemRectMin();
        ImVec2 mPosRelative = ImVec2(mPos.x - screenPos.x, mPos.y - screenPos.y);
        //Grid offsets to align any world position into one into the GUI, need to multiply by gridSize on the worldpos though.
        gridOffsetsX = screenPos.x - 1 + gridSize / 2 - (std::floor(offset.x) * gridSize);
        gridOffsetsY = screenPos.y - 1 + gridSize / 2 - (std::floor(offset.z) * gridSize);
        ImGui::Text("Position: %f, %f", mPosRelative.x, mPosRelative.y);
        if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            startMouse = ImVec2(Align(mPosRelative.x, gridSize), Align(mPosRelative.y, gridSize));
            if (mode == "WALL")  wallStart = glm::vec3(startMouse.x / gridSize, 0, startMouse.y / gridSize) + floor(offset);
            if (mode == "FLOOR") floorTL   = glm::vec3(startMouse.x / gridSize, 0, startMouse.y / gridSize) + floor(offset);
            if (mode == "TRIFLOOR") p1 = ImVec2(startMouse.x / gridSize + std::floor(offset.x), startMouse.y / gridSize + std::floor(offset.z));
        }
        if (isHovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            endMouse = ImVec2(Align(mPosRelative.x, gridSize), Align(mPosRelative.y, gridSize));
            if (mode == "WALL")  wallEnd = glm::vec3(endMouse.x / gridSize, 0, endMouse.y / gridSize) + floor(offset);
            if (mode == "FLOOR") floorBR = glm::vec3(endMouse.x / gridSize, 0, endMouse.y / gridSize) + floor(offset);
            if (mode == "TRIFLOOR") {
                p2OptionA = ImVec2(startMouse.x / gridSize + std::floor(offset.x), endMouse.y   / gridSize + std::floor(offset.z));
                p2OptionB = ImVec2(endMouse.x   / gridSize + std::floor(offset.x), startMouse.y / gridSize + std::floor(offset.z));
                p3 =        ImVec2(endMouse.x   / gridSize + std::floor(offset.x), endMouse.y   / gridSize + std::floor(offset.z));
            }
        }
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        // Show the map's walls, floors, and whatever else on the floorplan
        FloorplanRenderScene(drawList);
        // Grid
        for (int i = 0; i < ImGui::GetWindowHeight(); i += gridSize) {
            drawList->AddLine(ImVec2(screenPos.x, i + screenPos.y), ImVec2(ImGui::GetWindowWidth() + screenPos.x,  i + screenPos.y), ImColor(255, 255, 255, 25), 1);
        }
        for (int i = 0; i < ImGui::GetWindowWidth();  i += gridSize) {
            drawList->AddLine(ImVec2(i + screenPos.x, screenPos.y), ImVec2(i + screenPos.x, ImGui::GetWindowHeight() + screenPos.y), ImColor(255, 255, 255, 25), 1);
        }
        //ImGui::Text("Snapped: %f, %f", startMouse.x, startMouse.y);
        //ImGui::Text("Snapped: %f, %f", endMouse.x, endMouse.y);
        RenderGhost(drawList);
        RenderCursors();
        // Where is the player? Show that on the floorplan
        ImGui::SetNextItemAllowOverlap();
        glm::vec3 playerPos = Scene::_players[0].GetPos();
        ImGui::SetCursorPos(ImVec2(playerPos.x * gridSize - std::floor(offset.x) * gridSize, playerPos.z * gridSize - std::floor(offset.z) * gridSize));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 0, 0, 255));
        ImGui::Button("##player", ImVec2(gridSize, gridSize));
        ImGui::PopStyleColor();
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

//Render the To be placed object from the cursors
void UIHandler::RenderGhost(ImDrawList* drawList) {
    // Make a little line between the two points
    if (startMouse.x != -gridSize && endMouse.x != -gridSize) {
        if (mode == "WALL") drawList->AddLine(ImVec2(startMouse.x + screenPos.x + gridSize / 2, startMouse.y + screenPos.y + gridSize / 2),
            ImVec2(endMouse.x + screenPos.x + gridSize / 2, endMouse.y + screenPos.y + gridSize / 2),
            ImColor(0, 255, 0, 255), 1);
        if (mode == "FLOOR") drawList->AddRect(ImVec2(startMouse.x + screenPos.x + gridSize / 2, startMouse.y + screenPos.y + gridSize / 2),
            ImVec2(endMouse.x + screenPos.x + gridSize / 2, endMouse.y + screenPos.y + gridSize / 2),
            ImColor(0, 255, 0, 255));
        if (mode == "TRIFLOOR") {
            // Ugly and nonsenseical
            UIp1 =        ImVec2(startMouse.x + screenPos.x + gridSize / 2, startMouse.y + screenPos.y + gridSize / 2);
            UIp2OptionA = ImVec2(startMouse.x + screenPos.x + gridSize / 2, endMouse.y   + screenPos.y + gridSize / 2);
            UIp2OptionB = ImVec2(endMouse.x   + screenPos.x + gridSize / 2, startMouse.y + screenPos.y + gridSize / 2);
            UIp3 =        ImVec2(endMouse.x   + screenPos.x + gridSize / 2, endMouse.y   + screenPos.y + gridSize / 2);
            if (cycle) {
                drawList->AddTriangleFilled(
                    UIp1,
                    UIp2OptionA,
                    UIp3,
                    ImColor(0, 255, 0, 50));
                drawList->AddTriangle(
                    UIp1,
                    UIp2OptionB,
                    UIp3,
                    ImColor(0, 255, 0, 255));
            }
            else {
                drawList->AddTriangle(
                    UIp1,
                    UIp2OptionA,
                    UIp3,
                    ImColor(0, 255, 0, 255));
                drawList->AddTriangleFilled(
                    UIp1,
                    UIp2OptionB,
                    UIp3,
                    ImColor(0, 255, 0, 50));
            }
        }
    }
}
// It looks like a lot, it is, but all it does it show the tile where the mouse is pressed, released
void UIHandler::RenderCursors() {
    ImGui::SetNextItemAllowOverlap();
    ImGui::SetCursorPos(startMouse);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 255, 255, 255));
    ImGui::Button("##pressShow", ImVec2(gridSize, gridSize));
    ImGui::PopStyleColor();
    ImGui::SetNextItemAllowOverlap();
    ImGui::SetCursorPos(endMouse);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 255));
    ImGui::Button("##releaseShow", ImVec2(gridSize, gridSize));
    ImGui::PopStyleColor();
}

void UIHandler::FloorplanRenderScene(ImDrawList* dL) {
    for (Wall w : Scene::_walls) {
        // Start the line at where the wall would start in world space, then multiply it by gridsize in order to make it stay on the grid, then add the
        // current window's position to align it to the window, then add half the grid in order to center it within the windows. Then add an offset
        dL->AddLine(ImVec2(w.begin.x * gridSize + gridOffsetsX, w.begin.z * gridSize + gridOffsetsY),
            ImVec2(w.end.x * gridSize + gridOffsetsX, w.end.z * gridSize + gridOffsetsY),
            ImColor(255, 0, 0, 255), 1);
    }
    for (Floor f : Scene::_floors) {
        dL->AddRectFilled(ImVec2(f.topLeft.x * gridSize + gridOffsetsX, f.topLeft.z * gridSize + gridOffsetsY),
            ImVec2(f.bottomRight.x * gridSize + gridOffsetsX, f.bottomRight.z * gridSize + gridOffsetsY),
            ImColor(0, 255, 0, 50));
    }
    for (TriFloor tf : Scene::_trifloors) {
        dL->AddTriangleFilled(ImVec2(tf.p1.x * gridSize + gridOffsetsX, tf.p1.z * gridSize + gridOffsetsY),
            ImVec2(tf.p2.x * gridSize + gridOffsetsX, tf.p2.z * gridSize + gridOffsetsY),
            ImVec2(tf.p3.x * gridSize + gridOffsetsX, tf.p3.z * gridSize + gridOffsetsY),
            ImColor(0, 255, 0, 50));
    }
}

// Looks ugly so I put to bottom of file, problem solved!
void UIHandler::SwapModeButtons() {   
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(200, 15, 15, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(150, 30, 30, 255));
    if (ImGui::Button("Wall Mode")) {
        mode = "WALL";
        HideCursor();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0, 255, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(15, 200, 15, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(30, 150, 30, 255));
    if (ImGui::Button("Floor Mode")) {
        mode = "FLOOR";
        HideCursor();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0, 0, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(15, 15, 200, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(30, 30, 150, 255));
    if (ImGui::Button("Tri-Floor Mode")) {
        mode = "TRIFLOOR";
        HideCursor();
    }
    if (mode == "TRIFLOOR") {
        ImGui::SameLine();
        if (ImGui::Button("Cycle Tri-floor (C)"))
            cycle = !cycle;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}