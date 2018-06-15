#include "FileSelector.h"
#include "Image.h"
#include "Spritesheetmaker.h"
int main() {

	/*Image testImage("C:/Users/Nils/Downloads/Stabbing_Sword/frame0000.png");
	testImage.load();

	sf::Clock testTimer;
	//testImage.CropAndScale(.7f);

	//testImage.CropAndScale(.1f, true);

	//testImage.crop();
	//testImage.scale3(.1f);

	float time = testTimer.getElapsedTime().asSeconds();
	std::cout << "Test took " << time  << "s to complete." << std::endl;

	getchar();
	getchar();
	getchar();*/
	
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

	sf::Clock timePassed;

	std::vector<Image*> images;
	
	for (int i = 0; i < int(files.size()); i++) {
		Image * img = new Image(files[i]);
		if (!img->load()) {
			std::cout << "Unable to open file " << files[i] << "." << std::endl;
		}
		img->CropAndScale(scaleImagesBy, true);
		images.push_back(img);
	}

	SpriteSheet spriteSheet({ textureSizes[textureSizeSelected],textureSizes[textureSizeSelected] }, images);

	std::cout << "Made in " << timePassed.getElapsedTime().asSeconds() << "s" << std::endl;

	std::string saveTo = FileSelector::selectPath();

	if(saveTo != "none") spriteSheet.saveSpritesheet(saveTo);

	

	getchar();
	getchar();
	getchar();

	return 0;
}