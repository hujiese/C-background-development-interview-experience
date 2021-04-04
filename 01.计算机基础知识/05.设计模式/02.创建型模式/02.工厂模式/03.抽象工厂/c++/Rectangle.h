#pragma once
#include "Shape.h"
#include <iostream>
using namespace std;

class Rectangle :
	public Shape
{
public:
	Rectangle();
	virtual void draw();
	virtual ~Rectangle();
};

