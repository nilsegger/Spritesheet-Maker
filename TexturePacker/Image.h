#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <windows.h>

class Image {

public:
	Image(std::string file, float scaleAmount = 1.f);
	~Image();
	bool load();


	sf::Image * getImage() const;

	sf::Vector2u getSpriteSize();

	void cropIntoPixelArray();
	void scaleImageFromPixelArrayIntoImage();
	void scaleImageFromPixelArrayIntoImageMultiThread();

	void cropAndScale();

	void saveTo(std::string path);

	void merge(Image * other, bool isBackground = false);
private:
	std::string file;
	sf::Image * image;
	unsigned int borderX1, borderX2 = 0, borderY1, borderY2 = 0;
	unsigned int * pixelArray = nullptr;
	int pixelsCount = 0;
	float scaleAmount = 1.f;
private:
	bool pixelFits(sf::Image * img, unsigned int x, unsigned int y);
	bool hasPixel(unsigned int x, unsigned int y, sf::Image * img = nullptr);
	void setPixels(sf::Image * img, int index1, int index2, unsigned int offsetX, unsigned int offsetY, float scaleAmount);
	void setBorders();
	void setPixelArray();
	void merge(sf::Image * foreground, sf::Image * background, bool mergeOntoBackground);

};