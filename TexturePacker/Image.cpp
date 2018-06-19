#include "Image.h"

Image::Image(std::string file, float scaleAmount)
	:file(file), image(nullptr), scaleAmount(scaleAmount)
{
}

Image::~Image()
{
	delete image;
}

bool Image::load()
{
	image = new sf::Image;
	if (!image->loadFromFile(file)) return false;
	setBorders();
	return true;
}

void Image::setPixels(sf::Image * img, int index1, int index2, unsigned int offsetX, unsigned int offsetY, float scaleAmount)
{
	for (int i = index1; i < index2; i++) {
		int index = i * 6;
		unsigned int posx = unsigned int((pixelArray[index + 4] - offsetX) * scaleAmount);
		unsigned int posy = unsigned int((pixelArray[index + 5] - offsetY) * scaleAmount);

		if (posx < img->getSize().x && posy < img->getSize().y) {
			sf::Color color = sf::Color::Color(pixelArray[index], pixelArray[index + 1], pixelArray[index + 2], pixelArray[index + 3]);
			img->setPixel(posx, posy, color);
		}

	}
}

void Image::setBorders()
{
	borderX1 = image->getSize().x;
	borderY1 = image->getSize().y;
	for (unsigned int y = 0; y < image->getSize().y; y++) {
		for (unsigned int x = 0; x < image->getSize().x; x++) {
			if (hasPixel(x, y)) {
				if (x < borderX1) borderX1 = x;
				if (x > borderX2) borderX2 = x;
				if (y < borderY1) borderY1 = y;
				if (y > borderY2) borderY2 = y;
			}
		}
	}
}

void Image::setPixelArray()
{
	if (pixelArray != nullptr) delete pixelArray;
	pixelArray = new unsigned int[(borderX2 - borderX1) * (borderY2 - borderY1) * 6]; //rgba, xy
	pixelsCount = (borderX2 - borderX1) * (borderY2 - borderY1);
}

void Image::merge(sf::Image * foreground, sf::Image * background, bool mergeOntoBackground)
{
	if (foreground->getSize() != background->getSize()) {
		std::cout << "Images need to have the same size." << std::endl;
		return;
	}
	for (unsigned int y = 0; y < background->getSize().y; y++) {
		for (unsigned int x = 0; x < background->getSize().x; x++) {
			if (hasPixel(x, y, foreground)) {
				if(mergeOntoBackground) background->setPixel(x, y, foreground->getPixel(x,y));
			}
			else if (!mergeOntoBackground) {
				foreground->setPixel(x, y, background->getPixel(x, y));
			}
		}
	}
}

void Image::cropIntoPixelArray()
{
	setPixelArray();
	int index = 0;
	for (unsigned int y = borderY1; y < borderY2; y++) {
		for (unsigned int x = borderX1; x < borderX2; x++) {
			if (hasPixel(x, y)) {
				pixelArray[index * 6] = image->getPixel(x, y).r;
				pixelArray[index * 6 + 1] = image->getPixel(x, y).g;
				pixelArray[index * 6 + 2] = image->getPixel(x, y).b;
				pixelArray[index * 6 + 3] = image->getPixel(x, y).a;
				pixelArray[index * 6 + 4] = x;
				pixelArray[index * 6 + 5] = y;
				index++;
			}
		}
	}
	delete image;
	image = nullptr;
}

void Image::scaleImageFromPixelArrayIntoImage()
{
	if (image != nullptr) delete image;
	image = new sf::Image;
	sf::Vector2u scaledSize = getSpriteSize();
	image->create(scaledSize.x, scaledSize.y, sf::Color::Transparent);
	for (int i = 0; i < pixelsCount; i++) {
		int index = i * 6;
		unsigned int posx = unsigned int((pixelArray[index + 4] - borderX1) * scaleAmount);
		unsigned int posy = unsigned int((pixelArray[index + 5] - borderY1) * scaleAmount);

		if (posx < image->getSize().x && posy < image->getSize().y) {
			sf::Color color = sf::Color::Color(pixelArray[index], pixelArray[index + 1], pixelArray[index + 2], pixelArray[index + 3]);
			image->setPixel(posx, posy, color);
		}
	}
	delete pixelArray;
	pixelArray = nullptr;
}

void Image::scaleImageFromPixelArrayIntoImageMultiThread()
{
	if (image != nullptr) delete image;
	image = new sf::Image;
	sf::Vector2u scaledSize = getSpriteSize();
	image->create(scaledSize.x, scaledSize.y, sf::Color::Transparent);
	unsigned int splits = std::thread::hardware_concurrency();
	std::thread * threads[100];
	for (int i = 0; i < int(splits); i++) {
		int index1 = int(i * (pixelsCount / splits));
		int index2 = int(i * (pixelsCount / splits) + (pixelsCount / splits) - 1);
		threads[i] = new std::thread(&Image::setPixels, this, image, index1, index2, borderX1, borderY1, scaleAmount);

	}
	for (int i = 0; i < int(splits); i++) {
		threads[i]->join();
	}
	delete pixelArray;
	pixelArray = nullptr;
}

void Image::cropAndScale()
{
	cropIntoPixelArray();
	scaleImageFromPixelArrayIntoImageMultiThread();
}

void Image::saveTo(std::string path)
{
	image->saveToFile(path);
	delete image;
}

void Image::merge(Image * other, bool isBackground)
{
	if (isBackground) {
		merge(other->getImage(), this->getImage(), true);
	}
	else {
		merge(this->getImage(), other->getImage(), false);
	}
}

sf::Image * Image::getImage() const
{
	return image;
}

sf::Vector2u Image::getSpriteSize()
{
	return sf::Vector2u(unsigned int((borderX2 - borderX1) * scaleAmount), unsigned int((borderY2 - borderY1) * scaleAmount));
}

bool Image::pixelFits(sf::Image * img, unsigned int x, unsigned int y)
{
	return x >= 0 && y >= 0 && x < img->getSize().x && y < img->getSize().y;
}

bool Image::hasPixel(unsigned int x, unsigned int y, sf::Image * img)
{
	if(img == nullptr) return image->getPixel(x, y).a != 0.f;
	return img->getPixel(x, y).a != 0.f;
}



