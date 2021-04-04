#include "Mouse.h"


Mouse::Mouse()
{
}

void Mouse::accept(ComputerPartVisitor* computerPartVisitor)
{
	computerPartVisitor->visit(this);
}

Mouse::~Mouse()
{
}
