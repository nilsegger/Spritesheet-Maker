#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include "Image.h"

class Spritesheet {

public:
	Spritesheet();

	sf::Image * getSpritesheet();
	void saveSpritesheet(std::string path = "C:/Users/NILSEGGE/Pictures/spritesheet.png");
	bool prepareSpritesheet(sf::Vector2u spriteSize, unsigned int count, unsigned int maxWidth, unsigned int maxHeight, unsigned int padding = 0);
	void placeImage(Image * image, int index);

	sf::Vector2u getBiggestSpriteSize(std::vector<Image*> & images);
private:
	sf::Image * spritesheet = nullptr;
	sf::Vector2u spriteSize = { 0,0 };
	unsigned int spritesPerRow;
	unsigned int rowsCount;
private:
	sf::Vector2u getPosition(int index);
	void placeImage(sf::Image * image, unsigned int x, unsigned int y);
};