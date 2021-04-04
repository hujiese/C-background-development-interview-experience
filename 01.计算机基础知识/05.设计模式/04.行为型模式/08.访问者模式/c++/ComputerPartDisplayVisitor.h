#pragma once
#include "ComputerPartVisitor.h"
#include <iostream>
using namespace std;
class ComputerPartDisplayVisitor :
	public ComputerPartVisitor
{
public:
	ComputerPartDisplayVisitor();
	virtual void visit(Computer* computer);
	virtual void visit(Mouse* mouse);
	virtual void visit(Keyboard* keyboard);
	virtual void visit(Monitor* monitor);
	virtual ~ComputerPartDisplayVisitor();
};

