#include "FileSelector.h"
#include "Image.h"
int main() {

	Image img("C:/Users/NILSEGGE/frame0033/frame0000.png");

	img.load();

	img.crop();

	img.scale(0.5f);

	getchar();

	return 0;

	std::vector<std::string> files = FileSelector::selectFiles();
	if (files.size() == 0) return -1;
	std::cout << files.size() << " files selected." << std::endl;

	const int textueSizeCount = 3;
	int textureSizes[textueSizeCount] = { 1024, 2048, 4096 };
	int textureSizeSelected = -1;

	std::cout << "Size: ";
	for (int i = 0; i < textueSizeCount - 1; i++) std::cout << i << ")" << textureSizes[i] << ",";
	std::cout << textueSizeCount - 1 << ")" << textureSizes[textueSizeCount - 1] << std::endl;
	std::cin >> textureSizeSelected;

	int scaleImagesBy = -1;
	std::cout << "ScaleBy?" << std::endl;
	std::cin >> scaleImagesBy;

	std::cout << "Size: " << textureSizes[textureSizeSelected] << "x" << textureSizes[textureSizeSelected] << std::endl;
	std::cout << "Scale: " << scaleImagesBy  << std::endl;

	return 0;
}