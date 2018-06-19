#include "FileSelector.h"
#include "Image.h"
#include "Spritesheetmaker.h"


void imageWorkflow(Image * image, Spritesheet &spritesheet, int index);


int main() {
	sf::Clock testTimer;
	float scale = .3f;

	std::vector<Image*> images;

	std::vector<std::string> files = FileSelector::selectFiles();
	if (files.size() == 0) return -1;

	std::thread * threads[100];

	for (int i = 0; i < int(files.size()); i++) {
		images.push_back(new Image(files[i], scale));
		threads[i] = new std::thread(&Image::load, images[i]);
		//images[i]->load();
		/*images[i]->cropIntoPixelArray();
		images[i]->scaleImageFromPixelArrayIntoImageMultiThread();*/
	}
	for (int i = 0; i < int(files.size()); i++) {
		threads[i]->join();
		delete threads[i];
	}

	Spritesheet spritesheet;

	if(spritesheet.prepareSpritesheet(spritesheet.getBiggestSpriteSize(images), int(images.size()), 2048, 2048)) {
		for (int i = 0; i < int(files.size()); i++) {
			threads[i] = new std::thread(&imageWorkflow, images[i], spritesheet, i);
		}

		for (int i = 0; i < int(files.size()); i++) {
			threads[i]->join();
			delete threads[i];
		}
		float time = testTimer.getElapsedTime().asSeconds();
		std::cout << "Test took " << time << "s to complete." << std::endl;
		spritesheet.saveSpritesheet(FileSelector::selectPath());
	}
	getchar();
	getchar();
	
	return 0;
}

void imageWorkflow(Image * image, Spritesheet &spritesheet, int index) {
	image->cropIntoPixelArray();
	image->scaleImageFromPixelArrayIntoImageMultiThread();
	spritesheet.placeImage(image, index);
}