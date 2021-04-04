#include "AbstractFactory.h"
#include "ShapeFactory.h"
#include "ColorFactory.h"
#include "Color.h"
#include "Shape.h"
#include "Red.h"
#include "Green.h"
#include "Blue.h"
#include "Circle.h"
#include "Square.h"
#include "Rectangle.h"
#include "FactoryProducer.h"
#include <memory>

using namespace std;

int main(void)
{
	//��ȡ��״����
	shared_ptr<AbstractFactory> shapeFactory(FactoryProducer::getFactory("SHAPE"));

	//��ȡ��״Ϊ Circle �Ķ���
	shared_ptr<Shape> shape1(shapeFactory->getShape("CIRCLE"));

	//���� Circle �� draw ����
	shape1->draw();

	//��ȡ��״Ϊ Rectangle �Ķ���
	shared_ptr<Shape> shape2(shapeFactory->getShape("RECTANGLE"));

	//���� Rectangle �� draw ����
	shape2->draw();

	//��ȡ��״Ϊ Square �Ķ���
	shared_ptr<Shape> shape3(shapeFactory->getShape("SQUARE"));

	//���� Square �� draw ����
	shape3->draw();

	//��ȡ��ɫ����
	shared_ptr<AbstractFactory> colorFactory(FactoryProducer::getFactory("COLOR"));

	//��ȡ��ɫΪ Red �Ķ���
	shared_ptr<Color> color1(colorFactory->getColor("RED"));

	//���� Red �� fill ����
	color1->fill();

	//��ȡ��ɫΪ Green �Ķ���
	shared_ptr<Color> color2(colorFactory->getColor("GREEN"));

	//���� Green �� fill ����
	color2->fill();

	//��ȡ��ɫΪ Blue �Ķ���
	shared_ptr<Color> color3(colorFactory->getColor("BLUE"));

	//���� Blue �� fill ����
	color3->fill();

	return 0;
}