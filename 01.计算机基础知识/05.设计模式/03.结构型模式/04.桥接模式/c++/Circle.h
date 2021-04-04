#pragma once
#include "Shape.h"
class Circle :
	public Shape
{
public:
	Circle();
	Circle(int x, int y, int radius, shared_ptr<DrawAPI> drawAPI);
	void draw();
	virtual ~Circle();

private:
	int x, y, radius;
};

