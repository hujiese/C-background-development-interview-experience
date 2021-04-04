#pragma once
#include "ComputerPart.h"
#include "ComputerPartVisitor.h"
class Mouse :
	public ComputerPart
{
public:
	Mouse();
	virtual void accept(ComputerPartVisitor* computerPartVisitor);
	virtual ~Mouse();
};

