#pragma once

namespace Input {
	void  Init();
	void  Update();
	bool  KeyPressed(unsigned int key);
	bool  KeyDown(unsigned int key);
	float GetMouseOffsetX();
	float GetMouseOffsetY();
	bool  LeftMouseDown();
	bool  RightMouseDown();
	bool  LeftMousePressed();
	bool  RightMousePressed();
	bool  MouseWheelUp();
	bool  MouseWheelDown();
	int   GetMouseWheelValue();
	void  PreventRightMouseHold();
	int   GetMouseX();
	int   GetMouseY();
}