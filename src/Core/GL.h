#pragma once
#include "../Common.h"

namespace GL {
	void Init(int width, int height);
	void ProcessInput();
	bool WindowIsOpen();
	bool WindowIsOpen();
	int  GetWindowWidth();
	int  GetWindowHeight();
	int  GetCursorX();
	int  GetCursorY();
	void DisableCursor();
	void HideCursor();
	void ShowCursor();
	GLFWwindow* GetWindowPtr();
	int GetCursorScreenX();
	int GetCursorScreenY();

	enum WindowMode { WINDOWED, FULLSCREEN };
	void CreateWindow(WindowMode windowMode);
	void SetWindowMode(WindowMode windowMode);
	void ToggleFullscreen();
}