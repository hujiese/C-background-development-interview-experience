#include <stdio.h>

int main()
{
	register int a = 0;
	printf("register : &a = %x\n", &a); // register : &a = 8ffdf0 error C2103: 寄存器变量上的“&”

	return 0;
}
