#include "Image.h"

Image::Image(std::string file)
	:file(file), image(nullptr)
{
}

Image::~Image()
{
	delete image;
}

bool Image::load()
{
	image = new sf::Image;
	return image->loadFromFile(file);
}

void Image::crop()
{
	sf::Vector2u size = image->getSize();
	sf::Vector2u p1;
	sf::Vector2u p2;
	findTop(&p1.y);
	findBottom(&p2.y);
	findLeft(&p1.x);
	findRight(&p2.x);
	sf::Image * tempImage = new sf::Image;
	tempImage->create(p2.x - p1.x, p2.y - p1.y, sf::Color::White);
	for (unsigned int x = p1.x; x < p2.x; x++) {
		for (unsigned int y = p1.y; y < p2.y; y++) {
			tempImage->setPixel(x - p1.x, y - p1.y, image->getPixel(x, y));
		}
	}
	delete image;
	image = tempImage;
}

void Image::scale(float amount)
{

	sf::Image * tempImage = new sf::Image;
	tempImage->create(image->getSize().x * amount, image->getSize().y * amount, sf::Color::White);

	for (unsigned int y = 0; y < tempImage->getSize().y; y++) {
		for (unsigned int x = 0; x < tempImage->getSize().x; x++) {
			sf::Color pixelColor = image->getPixel(x / amount, y / amount);
			tempImage->setPixel(x, y, pixelColor);
		}
	}
	
	/*for (y = 0.f; unsigned int(y) < image->getSize().y; y += step) {
		for (x = 0.f; unsigned int(x) < image->getSize().x; x += step) {
			sf::Vector2u pixel = { unsigned int(x * amount), unsigned int(y * amount) };
			if (pixel.x >= tempImage->getSize().x || pixel.y >= tempImage->getSize().y) continue;
			tempImage->setPixel(pixel.x, pixel.y, image->getPixel(unsigned int(x),unsigned int(y)));
		}
	}*/
	delete image;
	image = tempImage;
}

sf::Image * Image::getImage() const
{
	return image;
}

void Image::findTop(unsigned int * top)
{
	for (unsigned int y = 0; y < image->getSize().y; y++) {
		for (unsigned int x = 0; x < image->getSize().x; x++) {
			if (image->getPixel(x, y).a != 0.f) {
				*top = y;
				return;
			}
		}
	}
}

void Image::findBottom(unsigned int * bottom)
{
	for (unsigned int y = image->getSize().y - 1; y >= 0; y--) {
		for (unsigned int x = 0; x < image->getSize().x; x++) {
			if (image->getPixel(x, y).a != 0.f) {
				*bottom = y;
				return;
			}
		}
	}
}

void Image::findLeft(unsigned int* left)
{
	for (unsigned int x = 0; x < image->getSize().x; x++) {
		for (unsigned int y = 0; y < image->getSize().y; y++) {
			if (image->getPixel(x, y).a != 0.f) {
				*left = x;
				return;
			}
		}
	}
}

void Image::findRight(unsigned int * right)
{
	for (unsigned int x = image->getSize().x - 1; x >= 0; x--) {
		for (unsigned int y = 0; y < image->getSize().y; y++) {
			if (image->getPixel(x, y).a != 0.f) {
				*right = x;
				return;
			}
		}
	}
}



