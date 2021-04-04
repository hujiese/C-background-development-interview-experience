#pragma once
#include "ComputerPart.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Monitor.h"

class Computer :
	public ComputerPart
{
public:
	Computer();
	virtual void accept(ComputerPartVisitor* computerPartVisitor);
	virtual ~Computer();

private:
	ComputerPart* parts[3];
};

