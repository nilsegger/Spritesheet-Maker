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

	sf::Image * spritesheet;

	void placeImages();
};