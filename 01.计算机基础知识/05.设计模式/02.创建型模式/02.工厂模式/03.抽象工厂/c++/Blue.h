#pragma once
#include "Color.h"
#include <iostream>

using namespace std;

class Blue :
	public Color
{
public:
	Blue();
	virtual void fill();
	virtual ~Blue();
};

