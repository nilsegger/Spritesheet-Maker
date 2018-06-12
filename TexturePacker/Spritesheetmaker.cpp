#include "Spritesheetmaker.h"

SpriteSheet::SpriteSheet(sf::Vector2u size, std::vector<Image*> images)
	:size(size), images(images)
{
	spritesheet = new sf::Image;
	spritesheet->create(size.x, size.y, sf::Color::White);
}
