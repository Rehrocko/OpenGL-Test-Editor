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