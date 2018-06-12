#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include "Image.h"

class SpriteSheet {

public:
	SpriteSheet(sf::Vector2u size, std::vector<Image*> images);

private:
	std::vector<Image*> images;
	sf::Vector2u size;

	unsigned int padding = 5;

	sf::Image * spritesheet;

	void placeImages();
	void placeImage(sf::Image * image, unsigned int x, unsigned int y);
	bool imageFits(sf::Image * image, unsigned int x, unsigned int y);
	void saveSpritesheet(std::string path = "C:/Users/Nils/Source/Repos/TexturePacker/TexturePacker/spritesheet.png");
};