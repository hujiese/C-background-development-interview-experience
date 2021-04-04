#pragma once
#include "AbstractFactory.h"
#include "Color.h"
#include "Red.h"
#include "Green.h"
#include "Blue.h"

class ColorFactory :
	public AbstractFactory
{
public:
	ColorFactory();
	virtual Color* getColor(string color);
	virtual ~ColorFactory();
};
