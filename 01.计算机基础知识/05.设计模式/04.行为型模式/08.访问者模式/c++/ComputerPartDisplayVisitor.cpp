#include "ComputerPartDisplayVisitor.h"


ComputerPartDisplayVisitor::ComputerPartDisplayVisitor()
{
}

void ComputerPartDisplayVisitor::visit(Computer* computer) {
	cout << "Displaying Computer." << endl;
}

void ComputerPartDisplayVisitor::visit(Mouse* mouse) {
	cout << "Displaying Mouse." << endl;
}

void ComputerPartDisplayVisitor::visit(Keyboard* keyboard) {
	cout << "Displaying Keyboard." << endl;
}

void ComputerPartDisplayVisitor::visit(Monitor* monitor) {
	cout << "Displaying Monitor." << endl;
}

ComputerPartDisplayVisitor::~ComputerPartDisplayVisitor()
{
}
