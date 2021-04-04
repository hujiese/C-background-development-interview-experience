#include "Keyboard.h"


Keyboard::Keyboard()
{
}

void Keyboard::accept(ComputerPartVisitor* computerPartVisitor)
{
	computerPartVisitor->visit(this);
}

Keyboard::~Keyboard()
{
}
