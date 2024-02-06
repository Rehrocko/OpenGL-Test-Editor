#pragma once

#include "Common.h"
#include <fstream>
#include <sstream> 

inline std::string ReadTextFromFile(std::string path) {
	std::ifstream file(path);
	std::string _string;
	std::string line;
	while (std::getline(file, line)) {
		_string += line + "\n";
	}
	return _string;
}

inline float Align(float value, float size)
{
	// Added std::abs to give correct behaviour for negative values
	return value - std::abs(std::fmod(value, size));
}

inline glm::vec3 Align(glm::vec3 value, float size) {
	glm::vec3 out;
	out.x = Align(value.x, size);
	out.y = Align(value.y, size);
	out.z = Align(value.z, size);
	return out;
}

inline glm::vec3 floor(glm::vec3 value) {
	return glm::vec3(std::floor(value.x), std::floor(value.y), std::floor(value.z));
}