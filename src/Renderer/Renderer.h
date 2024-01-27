#pragma once
#include "../Common.h"
#include "../util.hpp"

namespace Renderer {
	void Init();
	void RenderFrame();
	void ReloadShaders();
	void ToggleWireframe();
}