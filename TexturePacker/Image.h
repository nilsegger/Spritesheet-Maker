#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
class Image {

public:
	Image(std::string file);
	~Image();
	bool load();
	void crop();
	void scale(float amount);
	sf::Image * getImage() const;
private:
	std::string file;
	sf::Image * image;

	void findTop(unsigned int * top);
	void findBottom(unsigned int * bottom);
	void findLeft(unsigned int * left);
	void findRight(unsigned int * right);
};