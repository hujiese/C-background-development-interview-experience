#include <stdio.h>

int main()
{
	register int a = 0;
	printf("register : &a = %x\n", &a); // register : &a = 8ffdf0 error C2103: �Ĵ��������ϵġ�&��

	return 0;
}
