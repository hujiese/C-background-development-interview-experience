#include "Monitor.h"
#include "ComputerPartVisitor.h"

Monitor::Monitor()
{
}

void Monitor::accept(ComputerPartVisitor* computerPartVisitor)
{
	computerPartVisitor->visit(this);
}

Monitor::~Monitor()
{
}
