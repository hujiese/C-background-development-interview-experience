#pragma once
#include "ComputerPart.h"
#include "ComputerPartVisitor.h"
class Keyboard :
	public ComputerPart
{
public:
	Keyboard();
	virtual void accept(ComputerPartVisitor* computerPartVisitor);
	virtual ~Keyboard();
};

