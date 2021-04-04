#pragma once
#include "Shape.h"
#include <string>
#include <iostream>
using namespace std;

class Circle :
	public Shape
{
public:
	explicit Circle();
	Circle(const string color);
	void setX(const int x);
	void setY(const int y);
	void setRadius(const int radius);
	void draw();
	virtual ~Circle();

private:
	string color;
	int x;
	int y;
	int radius;
};
