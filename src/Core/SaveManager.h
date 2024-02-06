#pragma once
#include "../Common.h"
#include "Scene.h"
#include "GL.h"

namespace SaveManager {
	void SaveScene();
	void LoadScene();
	void LoadScene(std::string file);
	int  LoadWalls(int loopForOffset, int offset, std::byte* byteArr);
	int  LoadFloors(int loopForOffset, int offset, std::byte* byteArr);
	int  LoadTriFloors(int loopForOffset, int offset, std::byte* byteArr);
	int GetAllLengths();
	inline std::string filename;
}