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
