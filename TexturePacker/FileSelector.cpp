#include "FileSelector.h"

std::vector<std::string> FileSelector::selectFiles()
{
	OPENFILENAME ofn = { sizeof ofn };
	char file[1024];
	file[0] = '\0';
	ofn.lpstrFile = file;
	ofn.nMaxFile = 1024;
	ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	std::vector<std::string> files;

	if (GetOpenFileName(&ofn) == TRUE) {
		std::string directory = file;
		char * str = file;
		str += directory.size() + 1; //? + 1 wege NULL?
		if (*str != '\0') {
			while (*str) {
				std::string filename = str;
				str += filename.size() + 1;
				files.push_back(directory + "\\" + filename);
			}
		}
		else {
			files.push_back(directory);
		}
	}
	return files;
}

std::vector<std::string> FileSelector::orderFiles(std::vector<std::string> files)
{

	for (int i = 0; i < int(files.size()) - 1; i++) {
		if (getValue(files[i]) > getValue(files[i + 1])) {
			std::string temp = files[i];
			files[i] = files[i + 1];
			files[i + 1] = temp;
			i = 0;
		}
	}
	return files;
}

std::string FileSelector::selectPath()
{

	
	
	OPENFILENAME ofn = { sizeof ofn };
	char file[1024] = "Entire Folder";
	file[0] = '\0';
	ofn.lpstrFile = file;
	ofn.nMaxFile = 1024;
	ofn.Flags = OFN_EXPLORER;


	if (GetSaveFileName(&ofn) == TRUE) {
		return ofn.lpstrFile;
	}

	//Entire folder
	return "none";
}

int FileSelector::getValue(std::string str)
{
	std::string numbers = str.substr(str.length() - 8, 4); // - gnp.0000 + 0000
	return std::stoi(numbers);
}
