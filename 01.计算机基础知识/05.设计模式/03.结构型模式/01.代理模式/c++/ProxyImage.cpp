#include "ProxyImage.h"


ProxyImage::ProxyImage()
{
}

ProxyImage::ProxyImage(string fileName) :fileName(fileName)
{
}

void ProxyImage::display()
{
	if (realImage == NULL){
		realImage.reset(new RealImage(fileName));
	}
	realImage->display();
}

ProxyImage::~ProxyImage()
{
}
