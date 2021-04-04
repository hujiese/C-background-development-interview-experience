#include "ComputerPartDisplayVisitor.h"

int main(void)
{
	ComputerPart* computer = new Computer();
	computer->accept(new ComputerPartDisplayVisitor());

	delete computer;
	return 0;
}