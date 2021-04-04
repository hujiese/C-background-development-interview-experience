#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Rectangle.h"
#include "ShapeFactory.h"
#include <iostream>
#include <string>
#include <memory>
using namespace std;

int main(void)
{
	shared_ptr<ShapeFactory> shapeFactory(new ShapeFactory);

	//��ȡ Circle �Ķ��󣬲��������� draw ����
	shared_ptr<Shape> shape1(shapeFactory->getShape("CIRCLE"));

	//���� Circle �� draw ����
	shape1->draw();

	//��ȡ Rectangle �Ķ��󣬲��������� draw ����
	shared_ptr<Shape> shape2(shapeFactory->getShape("RECTANGLE"));

	//���� Rectangle �� draw ����
	shape2->draw();

	//��ȡ Square �Ķ��󣬲��������� draw ����
	shared_ptr<Shape> shape3(shapeFactory->getShape("SQUARE"));

	//���� Square �� draw ����
	shape3->draw();

	return 0;
}