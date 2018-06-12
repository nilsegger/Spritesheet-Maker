#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

class FileSelector {
public:
	static std::vector<std::string> selectFiles();
};