#include "Computer.h"

Computer::Computer()
{
	parts[0] = new Mouse();
	parts[1] = new Monitor();
	parts[2] = new Keyboard();
}

void Computer::accept(ComputerPartVisitor* computerPartVisitor)
{
	for (int i = 0; i < 3; i++) {
		parts[i]->accept(computerPartVisitor);
	}
	computerPartVisitor->visit(this);
}

Computer::~Computer()
{
}
