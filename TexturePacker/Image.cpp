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

	/*std::thread t0(&Image::findTop, this, &p1.y);
	std::thread t1(&Image::findBottom, this, &p2.y);
	std::thread t2(&Image::findLeft, this, &p1.x);
	std::thread t3(&Image::findRight, this, &p2.x);
	
	t0.join();
	t1.join();
	t2.join();
	t3.join();*/
	findTop(&p1.y);
	findBottom(&p2.y);
	findLeft(&p1.x);
	findRight(&p2.x);

	//if (!findBorders(&p1.x, &p2.x, &p1.y, &p2.y)) return;

	//std::cout << p1.x << "#" << p1.y << "-" << p2.x << "#" << p2.y << std::endl;

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
	if (amount == 1.f) return;
	sf::Image * tempImage = new sf::Image;
	tempImage->create(unsigned int(image->getSize().x * amount), unsigned int(image->getSize().y * amount), sf::Color::White);

	for (unsigned int y = 0; y < tempImage->getSize().y; y++) {
		for (unsigned int x = 0; x < tempImage->getSize().x; x++) {
			sf::Color pixelColor = image->getPixel(unsigned int(x / amount), unsigned int(y / amount));
			tempImage->setPixel(x, y, pixelColor);
		}
	}

	delete image;
	image = tempImage;
}

void Image::scale2(float amount)
{
	sf::Image * tempImage = new sf::Image;
	tempImage->create(unsigned int(image->getSize().x * amount), unsigned int(image->getSize().y * amount), sf::Color::White);

	for (unsigned int y = 0; y < tempImage->getSize().y; y++) {
		for (unsigned int x = 0; x < tempImage->getSize().x; x++) {

			std::vector<sf::Vector2u> pixels = getNeighbourPixels(tempImage, x, y, amount);
			
			unsigned int r = 0, g = 0, b = 0, a = 0;

			for (int i = 0; i < int(pixels.size()); i++) {
				r += image->getPixel(pixels[i].x, pixels[i].y).r;
				g += image->getPixel(pixels[i].x, pixels[i].y).g;
				b += image->getPixel(pixels[i].x, pixels[i].y).b;
				a += image->getPixel(pixels[i].x, pixels[i].y).a;
			}

			r /= unsigned int(pixels.size());
			g /= unsigned int(pixels.size());
			b /= unsigned int(pixels.size());
			a /= unsigned int(pixels.size());

			sf::Color pixelColor = sf::Color::Color(r,g,b,a);
			tempImage->setPixel(x, y, pixelColor);

		}

	}

	delete image;
	image = tempImage;
}

void Image::scale3(float amount)
{
	
	unsigned int splits = std::thread::hardware_concurrency();
		
	if (splits == 1) {
		scale(amount);
		return;
	}

	if (amount == 1.f) return;
	sf::Image * tempImage = new sf::Image;
	tempImage->create(unsigned int(image->getSize().x * amount), unsigned int(image->getSize().y * amount), sf::Color::White);

	std::thread * threads[100];
	
	unsigned int ySize = tempImage->getSize().y / splits;

	for (unsigned int i = 0; i < splits; i++) {
		//std::cout << "Y1 " << i * ySize << " Y2 " << i * ySize + ySize - 1 << std::endl;
		threads[i] = new std::thread(&Image::scale3placePixels, this, tempImage, i * ySize, i * ySize + ySize - 1, amount);
	}


	for (unsigned int i = 0; i < splits; i++) {
		threads[i]->join();
		delete threads[i];
	}

	/*for (unsigned int y = 0; y < tempImage->getSize().y; y++) {
		for (unsigned int x = 0; x < tempImage->getSize().x; x++) {
			sf::Color pixelColor = image->getPixel(unsigned int(x / amount), unsigned int(y / amount));
			tempImage->setPixel(x, y, pixelColor);
		}
	}*/

	delete image;
	image = tempImage;
}

void Image::scale3placePixels(sf::Image * tempImage, unsigned int y1, unsigned int y2, float amount)
{

	for (unsigned int y = y1; y < y2; y++) {
		for (unsigned int x = 0; x < tempImage->getSize().x; x++) {
			tempImage->setPixel(x, y, image->getPixel(unsigned int(x / amount), unsigned int(y / amount)));
		}
	}
}

void Image::setPixels(sf::Image * img, int index1, int index2, unsigned int offsetX, unsigned int offsetY, float scaleAmount)
{
	for (int i = index1; i < index2; i++) {
		int index = i * 6;
		int posx = unsigned int((pixelArray[index + 4] - offsetX) * scaleAmount);
		int posy = unsigned int((pixelArray[index + 5] - offsetY) * scaleAmount);

		if (posx < img->getSize().x && posy < img->getSize().y) {
			sf::Color color = sf::Color::Color(pixelArray[index], pixelArray[index + 1], pixelArray[index + 2], pixelArray[index + 3]);
			img->setPixel(posx, posy, color);
		}

	}
}

void Image::CropAndScale(float scaleAmount, bool multithread)
{

	pixelArray = new unsigned int[image->getSize().x * image->getSize().y * 6]; //rgba, xy

	

	unsigned int x1 = image->getSize().x, x2 = 0, y1 = image->getSize().y, y2 = 0;

	for (unsigned int y = 0; y < image->getSize().y; y++) {
		for (unsigned int x = 0; x < image->getSize().x; x++) {
			if (hasPixel(x, y)) {
				if (x < x1) x1 = x;
				if (x > x2) x2 = x;
				if (y < y1) y1 = y;
				if (y > y2) y2 = y;
				pixelArray[pixelsCount * 6] = image->getPixel(x, y).r;
				pixelArray[pixelsCount * 6 + 1] = image->getPixel(x, y).g;
				pixelArray[pixelsCount * 6 + 2] = image->getPixel(x, y).b;
				pixelArray[pixelsCount * 6 + 3] = image->getPixel(x, y).a;
				pixelArray[pixelsCount * 6 + 4] = x;
				pixelArray[pixelsCount * 6 + 5] = y;
				/*std::cout << x << "#" << y << pixelArray[pixelsCount * 6 + 4] << pixelArray[pixelsCount * 6 + 5] << std::endl;
				getchar();*/
				pixelsCount++;
			}
		}
	}

	//std::cout << x1 << "#" << y1 << "-" << x2 << "#" << y2 << std::endl;

	sf::Image * tempImg = new sf::Image;
	tempImg->create(unsigned int((x2 - x1) * scaleAmount), unsigned int((y2 - y1) * scaleAmount), sf::Color::Transparent);

	/*

	Upscale or downscale

	*/
	if (multithread) {
		unsigned int splits = std::thread::hardware_concurrency();

		std::thread * threads[100];

		for (int i = 0; i < splits; i++) {
			int index1 = int(i * (pixelsCount / splits));
			int index2 = int(i * (pixelsCount / splits) + (pixelsCount / splits) - 1);

			//sf::Image * img, int index1, int index2, unsigned int offsetX, unsigned int offsetY, float scaleAmount

			threads[i] = new std::thread(&Image::setPixels, this, tempImg, index1, index2, x1, y1, scaleAmount);

		}

		for (int i = 0; i < splits; i++) {
			threads[i]->join();
		}
	}
	else {
		for (int i = 0; i < pixelsCount; i++) {
			int index = i * 6;
			int posx = unsigned int((pixelArray[index + 4] - x1) * scaleAmount);
			int posy = unsigned int((pixelArray[index + 5] - y1) * scaleAmount);

			if (posx < tempImg->getSize().x && posy < tempImg->getSize().y) {
				sf::Color color = sf::Color::Color(pixelArray[index], pixelArray[index + 1], pixelArray[index + 2], pixelArray[index + 3]);
				tempImg->setPixel(posx, posy, color);
			}
		}
	}
	delete image;
	image = tempImg;
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



std::vector<sf::Vector2u> Image::getNeighbourPixels(sf::Image * img, unsigned int x, unsigned int y, float step)
{
	std::vector<sf::Vector2u> neighbours;
	neighbours.push_back({x,y});
	neighbours.push_back({ x-1,y-1 });
	neighbours.push_back({ x,y - 1 });
	neighbours.push_back({ x+1,y - 1 });
	neighbours.push_back({ x-1,y });
	neighbours.push_back({ x+1,y});
	neighbours.push_back({ x-1,y + 1 });
	neighbours.push_back({ x,y + 1 });
	neighbours.push_back({ x+1,y + 1 });

	std::vector<sf::Vector2u> scaled;

	for (int i = 0; i < int(neighbours.size()); i++) {
		if (pixelFits(img, neighbours[i].x, neighbours[i].y)) {
			if (pixelFits(image, unsigned int(neighbours[i].x / step), unsigned int(neighbours[i].y / step))) {
				scaled.push_back({ unsigned int(neighbours[i].x / step), unsigned int(neighbours[i].y / step) });
			}
		}
	}

	if (scaled.size() == 0) return std::vector<sf::Vector2u>();

	unsigned int outerX = scaled[0].x, outerY = scaled[0].y, innerX = scaled[0].x, innerY = scaled[0].y;

	for (int i = 1; i < int(scaled.size()); i++) {
		if (scaled[i].x < innerX) innerX = scaled[i].x;
		if (scaled[i].y < innerY) innerY = scaled[i].y;
		if (scaled[i].x > outerX) outerX = scaled[i].x;
		if (scaled[i].y > outerY) outerY = scaled[i].y;
	}

	std::vector<sf::Vector2u> pixels;

	for (unsigned int iy = innerY; iy < outerY; iy++) {
		for (unsigned int ix = innerX; ix < outerX; ix++) {
			pixels.push_back({ ix,iy });
		}
	}

	return pixels;
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



