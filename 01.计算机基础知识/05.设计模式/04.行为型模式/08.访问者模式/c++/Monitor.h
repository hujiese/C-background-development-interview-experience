#pragma once
#include "ComputerPart.h"
#include "ComputerPartVisitor.h"
class Monitor :
	public ComputerPart
{
public:
	Monitor();
	virtual void accept(ComputerPartVisitor* computerPartVisitor);
	virtual ~Monitor();
};

