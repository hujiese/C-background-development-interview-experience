#pragma once
#include "Color.h"
#include <iostream>

using namespace std;
class Green :
	public Color
{
public:
	Green();
	virtual void fill();
	virtual ~Green();
};

