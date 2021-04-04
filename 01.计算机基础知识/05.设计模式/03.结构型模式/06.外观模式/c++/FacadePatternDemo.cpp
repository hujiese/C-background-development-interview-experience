#include <iostream>
#include "ShapeMaker.h"
using namespace std;

int main(void)
{
	shared_ptr<ShapeMaker> shapeMaker(new ShapeMaker());

	shapeMaker->drawCircle();
	shapeMaker->drawRectangle();
	shapeMaker->drawSquare();

	return 0;
}