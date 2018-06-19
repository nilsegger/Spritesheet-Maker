#include "FileSelector.h"
#include "Image.h"
#include "Spritesheetmaker.h"

int main() {
	sf::Clock testTimer;
	float scale = .3f;

	std::vector<Image*> images;

	std::vector<std::string> files = FileSelector::selectFiles();
	if (files.size() == 0) return -1;

	for (int i = 0; i < int(files.size()); i++) {
		images.push_back(new Image(files[i], scale));
		images[i]->load();
		images[i]->cropIntoPixelArray();
		images[i]->scaleImageFromPixelArrayIntoImageMultiThread();
	}

	Spritesheet spritesheet;
	if (spritesheet.prepareSpritesheet(spritesheet.getBiggestSpriteSize(images), int(images.size()), 2048, 2048)) {

		for (int i = 0; i < int(images.size()); i++) {
			spritesheet.placeImage(images[i], i);
		}


		float time = testTimer.getElapsedTime().asSeconds();
		std::cout << "Test took " << time << "s to complete." << std::endl;

		spritesheet.saveSpritesheet(FileSelector::selectPath());
	}
	
	getchar();
	getchar();
	
	return 0;
}
