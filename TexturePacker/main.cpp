#include "FileSelector.h"
#include "Image.h"
#include "Spritesheetmaker.h"
#include "PackedSpritesheet.h"

void imageWorkflow(Image * image, Spritesheet &spritesheet, int index);


int main() {

	float scale = .4f;

	sf::Clock testTimer;
	std::vector<Image*> images;

	std::vector<std::string> files = FileSelector::orderFiles( FileSelector::selectFiles() );

	/*for (int i = 0; i < files.size(); i++) {
		std::cout << files[i] << std::endl;
	}

	getchar();
	getchar();
	getchar();*/

	if (files.size() == 0) return -1;

	std::thread * threads[100];

	for (int i = 0; i < int(files.size()); i++) {
		images.push_back(new Image(files[i], scale));
		threads[i] = new std::thread(&Image::load, images[i]);
		//images[i]->load();
	}
	for (int i = 0; i < int(files.size()); i++) {
		threads[i]->join();
		delete threads[i];
	}

	for (int i = 0; i < int(files.size()); i++) {
		images[i]->cropIntoPixelArray();
		images[i]->scaleImageFromPixelArrayIntoImageMultiThread();
	}

	PackedSpritesheet spritesheet({2048, 2048}, images);
	spritesheet.saveSpritesheet(FileSelector::selectPath());


	/*Spritesheet spritesheet;

	if(spritesheet.prepareGridSpritesheet(spritesheet.getBiggestSpriteSize(images), int(images.size()), 2048, 2048)) {
		for (int i = 0; i < int(files.size()); i++) {
			images[i]->cropIntoPixelArray();
			images[i]->scaleImageFromPixelArrayIntoImageMultiThread();
			spritesheet.placeImage(images[i], i);
			//threads[i] = new std::thread(&imageWorkflow, images[i], spritesheet, i);
		}

		float time = testTimer.getElapsedTime().asSeconds();
		std::cout << "Test took " << time << "s to complete." << std::endl;
		spritesheet.saveSpritesheet(FileSelector::selectPath());
	}*/

	getchar();
	getchar();
	
	return 0;
}

void imageWorkflow(Image * image, Spritesheet &spritesheet, int index) {
	image->cropIntoPixelArray();
	image->scaleImageFromPixelArrayIntoImageMultiThread();
	spritesheet.placeImage(image, index);
}