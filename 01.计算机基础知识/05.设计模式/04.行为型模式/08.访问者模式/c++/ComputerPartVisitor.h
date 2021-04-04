#pragma once
#include "ComputerPart.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Monitor.h"
#include "Computer.h"

class Mouse;
class Keyboard;
class Monitor;
class Computer;

class ComputerPartVisitor
{
public:
	ComputerPartVisitor();
	virtual void visit(Computer* computer) = 0;
	virtual void visit(Mouse* mouse) = 0;
	virtual void visit(Keyboard* keyboard) = 0;
	virtual void visit(Monitor* monitor) = 0;
	virtual ~ComputerPartVisitor();
};

