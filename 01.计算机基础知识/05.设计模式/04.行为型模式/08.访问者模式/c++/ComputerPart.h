#pragma once
class ComputerPartVisitor;

class ComputerPart
{
public:
	ComputerPart();
	virtual void accept(ComputerPartVisitor* computerPartVisitor) = 0;
	virtual ~ComputerPart();
};

