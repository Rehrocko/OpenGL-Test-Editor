#pragma once
#include "../Common.h"
#include "Scene.h"
#include "GL.h"
#include "SaveManager.h"

namespace UIHandler {
	void Update(float deltaTime);
	void Render();
	void Editor();
	void HideCursor();
	void RenderCursors();
	void FloorplanRenderScene(ImDrawList* dL);
	void CreateWall();
	void CreateFloor();
	void CreateTriFloor();
	void Floorplan();
	void SceneButtons();
	void SwapModeButtons();
	void RenderGhost(ImDrawList* drawList);
	inline bool _openFloorPlan = false;
	inline std::string mode = "WALL";
	inline ImVec2 startMouse, endMouse;
	inline float floorPlanSpeed = 0.05f;
	inline glm::vec3 offset;
	inline int gridSize = 20;
	// Stuff for all the editors
	inline float gridOffsetsX, gridOffsetsY;
	inline ImVec2 screenPos;
	// Stuff for wall editor
	inline glm::vec3 wallStart(0);
	inline glm::vec3 wallEnd(0);
	// Stuff for floor editor
	inline glm::vec3 floorTL(0);
	inline glm::vec3 floorBR(0);
	inline float levels[4];
	// Stuff for trifloor editor
	inline float tlevels[3]; /* Levels for trifloor */
	inline ImVec2 UIp1, UIp2OptionA, UIp2OptionB, UIp3, p1, p2OptionA, p2OptionB, p3;
	inline bool cycle; /* I don't know what the variable name should be but it just cycles if the user chooses a certain trifloor */
}