#include "FileSelector.h"
#include "Image.h"
#include "Spritesheetmaker.h"
int main() {

	std::vector<std::string> files = FileSelector::selectFiles();
	if (files.size() == 0) return -1;
	std::cout << files.size() << " files selected." << std::endl;

	const int textueSizeCount = 3;
	unsigned int textureSizes[textueSizeCount] = { 1024, 2048, 4096 };
	int textureSizeSelected = -1;

	std::cout << "Size: ";
	for (int i = 0; i < textueSizeCount - 1; i++) std::cout << i << ")" << textureSizes[i] << ",";
	std::cout << textueSizeCount - 1 << ")" << textureSizes[textueSizeCount - 1] << std::endl;
	std::cin >> textureSizeSelected;

	float scaleImagesBy = -1;
	std::cout << "ScaleBy?" << std::endl;
	std::cin >> scaleImagesBy;

	std::cout << "Size: " << textureSizes[textureSizeSelected] << "x" << textureSizes[textureSizeSelected] << std::endl;
	std::cout << "Scale: " << scaleImagesBy  << std::endl;

	std::vector<Image*> images;
	
	for (int i = 0; i < files.size(); i++) {
		Image * img = new Image(files[i]);
		if (!img->load()) {
			std::cout << "Unable to open file" << std::endl;
		}
		img->crop();
		img->scale(scaleImagesBy);
		images.push_back(img);
	}

	SpriteSheet spriteSheet({ textureSizes[textureSizeSelected],textureSizes[textureSizeSelected] }, images);

	return 0;
}