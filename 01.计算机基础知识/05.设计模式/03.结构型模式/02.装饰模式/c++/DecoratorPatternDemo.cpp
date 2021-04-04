#include <iostream>
#include "Circle.h"
#include "Rectangle.h"
#include "RedShapeDecorator.h"
using namespace std;

int main(void)
{
	shared_ptr<Shape> circle(new Circle());

	shared_ptr<Shape> redShapeDecorator(new Circle());
	shared_ptr<Shape> redCircle(redShapeDecorator);

	shared_ptr<Shape> redShapeDecorator2(new Rectangle());
	shared_ptr<Shape> redRectangle(redShapeDecorator2);

	cout << "Circle with normal border" << endl;
	circle->draw();

	cout << endl;

	cout << "Circle of red border" << endl;
	redCircle->draw();

	cout << endl;
	cout << "Rectangle of red border" << endl;
	redRectangle->draw();

	return 0;
}