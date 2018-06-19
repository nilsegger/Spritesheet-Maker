#include "Spritesheetmaker.h"

Spritesheet::Spritesheet()
	
{
}

sf::Image * Spritesheet::getSpritesheet()
{
	return spritesheet;
}


sf::Vector2u Spritesheet::getPosition(int index)
{
	if (spritesPerRow == 0 || rowsCount == 0) {
		std::cout << "spritesPerRow or rowsCount is 0" << std::endl;
		return sf::Vector2u(0, 0);
	}

	unsigned int x = 0, y = 0;

	for (int i = 0; i < index; i++) {
		x++;
		if (x >= spritesPerRow) {
			x = 0;
			y++;
		}
	}
	
	x = x * spriteSize.x;
	y = y * spriteSize.y;

	//Gives me too much problems
	/*x = index % spritesPerRow * spriteSize.x;
	//if (index % spritesPerRow >= 1) x -= spriteSize.x;
	y = unsigned int(index / rowsCount) *  spriteSize.y;
	std::cout<< index << "#" << x << "#" << y << std::endl;*/
	return sf::Vector2u(x,y);
}

void Spritesheet::placeImage(sf::Image * image, unsigned int x, unsigned int y)
{
	for (unsigned int iy = 0; iy < image->getSize().y; iy++) {
		for (unsigned int ix = 0; ix < image->getSize().x; ix++) {
			spritesheet->setPixel(ix + x, iy + y, image->getPixel(ix, iy));
		}
	}
}

void Spritesheet::saveSpritesheet(std::string path)
{
	std::cout << "Saving..." << std::endl;
	if (spritesheet->saveToFile(path)) {

		std::ofstream file(path + ".info.txt");
		file << "Grid Dimensions.\n X: " << spriteSize.x << "#" << spriteSize.y;
		file.close();
		std::cout << "Saved spritesheet to " << path << std::endl;
	}
	else std::cout << "Failed to save spritesheet." << std::endl;
	

	delete spritesheet;
}

bool Spritesheet::prepareSpritesheet(sf::Vector2u spriteSize, unsigned int count, unsigned int maxWidth, unsigned int maxHeight, unsigned int padding)
{
	if (spritesheet != nullptr) delete spritesheet;

	this->spriteSize = { spriteSize.x + 2 * padding, spriteSize.y + 2 * padding };

	spritesPerRow = unsigned int(maxWidth / this->spriteSize.x);
	rowsCount = unsigned int(maxHeight / this->spriteSize.y);

	if (rowsCount * spritesPerRow < count) {
		std::cout << "Sprites wont fit" << std::endl;
		return false;
	}

	sf::Vector2u size = sf::Vector2u(spritesPerRow * this->spriteSize.x /*+ this->spriteSize.x*/, getPosition(count).y + this->spriteSize.y);

	if (count < spritesPerRow) {
		size.x = spriteSize.x * count;
	}

	spritesheet = new sf::Image;
	spritesheet->create(size.x, size.y, sf::Color::Transparent);
	return true;
}

void Spritesheet::placeImage(Image * image, int index)
{
	sf::Vector2u position = getPosition(index);
	sf::Vector2u centeringOffset = (spriteSize - image->getImage()->getSize());
	centeringOffset.x /= 2;
	centeringOffset.y /= 2;
	placeImage(image->getImage(), position.x + centeringOffset.x, position.y + centeringOffset.y);
}

sf::Vector2u Spritesheet::getBiggestSpriteSize(std::vector<Image*> & images)
{
	sf::Vector2u size = { 0,0 };
	for (int i = 0; i < int(images.size()); i++) {
		if (images[i]->getSpriteSize().x > size.x) size.x = images[i]->getSpriteSize().x;
		if (images[i]->getSpriteSize().y > size.y) size.y = images[i]->getSpriteSize().y;
	}
	return size;
}

