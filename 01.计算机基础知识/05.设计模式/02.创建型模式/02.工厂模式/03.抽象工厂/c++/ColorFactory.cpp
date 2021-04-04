#include "ColorFactory.h"


ColorFactory::ColorFactory()
{
}

Color* ColorFactory::getColor(string color)
{
	if (color == ""){
		return NULL;
	}
	if (color == "RED"){
		return new Red();
	}
	else if (color == "GREEN"){
		return new Green();
	}
	else if (color == "BLUE"){
		return new Blue();
	}
	return NULL;
}

ColorFactory::~ColorFactory()
{
}
