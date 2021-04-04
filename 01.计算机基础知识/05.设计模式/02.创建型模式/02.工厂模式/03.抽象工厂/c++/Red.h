#pragma once
#include "Color.h"
#include <iostream>
using namespace std;

class Red :
	public Color
{
public:
	Red();
	virtual void fill();
	virtual ~Red();
};

