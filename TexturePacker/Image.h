#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <windows.h>

class Image {

public:
	Image(std::string file);
	~Image();
	bool load();
	void crop();
	void scale(float amount);
	void scale2(float amount);
	void scale3(float amount);
	


	sf::Image * getImage() const;

	void CropAndScale(float scaleAmount, bool multithread = false);
private:
	std::string file;
	sf::Image * image;

	void findTop(unsigned int * top);
	void findBottom(unsigned int * bottom);
	void findLeft(unsigned int * left);
	void findRight(unsigned int * right);

	std::vector<sf::Vector2u> getNeighbourPixels(sf::Image * img, unsigned int x, unsigned int y, float step);
	bool pixelFits(sf::Image * img, unsigned int x, unsigned int y);
	bool hasPixel(unsigned int x, unsigned int y, sf::Image * img = nullptr);
	void scale3placePixels(sf::Image * tempImage, unsigned int y1, unsigned int y2, float amount);

	
	unsigned int * pixelArray;
	int pixelsCount = 0;

	void setPixels(sf::Image * img, int index1, int index2, unsigned int offsetX, unsigned int offsetY, float scaleAmount);
	
	static unsigned int maxThreadsAtOnce;
	static unsigned int currentThreads;
};