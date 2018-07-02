#include "PackedSpritesheet.h"

PackedSpritesheet::PackedSpritesheet(sf::Vector2u size, std::vector<Image*> images)
	:size(size), images(images)
{
	spritesheet = new sf::Image;
	spritesheet->create(size.x, size.y, sf::Color::Transparent);
	placeImages();
}

sf::Image * PackedSpritesheet::getSpritesheet()
{
	return spritesheet;
}

void PackedSpritesheet::placeImages()
{

	std::thread * placeImagesThreads[100];
	int threadCount = 0;

	unsigned int x = 0, y = 0;
	unsigned int rowHeight = 0;
	for (int i = 0; i < int(images.size()); i++) {
		sf::Image * image = images[i]->getImage();

		if (!imageFits(image, x, y)) {
			y += rowHeight + padding;
			rowHeight = 0;
			x = 0;
			if (imageFits(image, x, y)) {
				placeImagesThreads[threadCount] = new std::thread(&PackedSpritesheet::placeImage, this, image, x, y);
				threadCount++;
				//placeImage(image, x, y);
				rowHeight = image->getSize().y;
				x += padding + image->getSize().x;
				rowHeight = image->getSize().y;
			}
			else {
				std::cout << "Images dont fit... At image " << i + 1 << "of" << int(images.size()) << std::endl;
				saveSpritesheet();
				return;
			}
		}
		else if (imageFits(image, x, y)) {
			placeImagesThreads[threadCount] = new std::thread(&PackedSpritesheet::placeImage, this, image, x, y);
			threadCount++;
			//placeImage(image, x, y);
			x += padding + image->getSize().x;
			if (image->getSize().y > rowHeight) rowHeight = image->getSize().y;
		}
		/*
		Place pixels
		set offset
		check whetever everything fits
		padding?
		*/
		//delete image;
	}

	for (int i = 0; i < threadCount; i++) {
		placeImagesThreads[i]->join();
	}

	//saveSpritesheet();
}

void PackedSpritesheet::placeImage(sf::Image * image, unsigned int x, unsigned int y)
{
	for (unsigned int ix = 0; ix < image->getSize().x; ix++) {
		for (unsigned int iy = 0; iy < image->getSize().y; iy++) {
			spritesheet->setPixel(x + ix, y + iy, image->getPixel(ix, iy));
		}
	}
}

bool PackedSpritesheet::imageFits(sf::Image * image, unsigned int x, unsigned int y)
{
	return (x + image->getSize().x < spritesheet->getSize().x && y + image->getSize().y < spritesheet->getSize().y);
}

void PackedSpritesheet::saveSpritesheet(std::string path)
{
	std::cout << "Saving..." << std::endl;
	if (spritesheet->saveToFile(path)) std::cout << "Saved spritesheet to " << path << std::endl;
	else std::cout << "Failed to save spritesheet." << std::endl;
	delete spritesheet;
	for (int i = 0; i < int(images.size()); i++) {
		delete images[i];
	}
}