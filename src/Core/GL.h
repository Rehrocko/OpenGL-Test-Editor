#pragma once
#include "../Common.h"

namespace GL {
	void Init(int width, int height);
	void ProcessInput();
	void SwapBuffersPollEvents();
	void Terminate();
	bool WindowIsOpen();
	int  GetWindowWidth();
	int  GetWindowHeight();
	int  GetCursorX();
	int  GetCursorY();
	void DisableCursor();
	void HideCursor();
	void ShowCursor();
	void ToggleCursor();
	GLFWwindow* GetWindowPtr();
	int GetCursorScreenX();
	int GetCursorScreenY();
	bool WindowHasFocus();
	bool WindowHasNotBeenForceClosed();
	void ForceCloseWindow();

	enum WindowMode { WINDOWED, FULLSCREEN };
	void CreateWindow(WindowMode windowMode);
	void SetWindowMode(WindowMode windowMode);
	void ToggleFullscreen();
}