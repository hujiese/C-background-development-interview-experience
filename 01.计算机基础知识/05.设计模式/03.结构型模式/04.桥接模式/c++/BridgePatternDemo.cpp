#include <iostream>
#include <memory>
#include "Circle.h"
#include "GreenCircle.h"
#include "RedCircle.h"

using namespace std;

int main(void)
{
	shared_ptr<DrawAPI> red(new RedCircle());
	shared_ptr<DrawAPI> green(new RedCircle());
	shared_ptr<Shape> redCircle(new Circle(100, 100, 10, red));
	shared_ptr<Shape> greenCircle(new Circle(100, 100, 10, green));

	redCircle->draw();
	greenCircle->draw();

	return 0;
}