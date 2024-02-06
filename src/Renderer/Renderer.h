#pragma once
#include "../Common.h"
#include "../util.hpp"
#include "../Core/Player.h"

namespace Renderer {
	void Init();
	void RenderFrame(Player* player);
	void ReloadShaders();

	glm::mat4 GetProjectionMatrix(float depthOfField);
}