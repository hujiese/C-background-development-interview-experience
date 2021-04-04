#include "RealImage.h"


RealImage::RealImage()
{
}

RealImage::RealImage(string fileName) :fileName(fileName)
{
	loadFromDisk(fileName);
}

void RealImage::display()
{
	cout << "Displaying " << fileName << endl;
}

void RealImage::loadFromDisk(string fileName)
{
	cout << "Loading " << fileName << endl;
}

RealImage::~RealImage()
{
}
