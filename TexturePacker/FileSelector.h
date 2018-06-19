#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

class FileSelector {
public:
	static std::vector<std::string> selectFiles();
	static std::vector<std::string> orderFiles(std::vector<std::string> files);
	static std::string selectPath();

private:
	static int getValue(std::string str);
};