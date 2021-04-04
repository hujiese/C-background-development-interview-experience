#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("./a.out number");
		exit(1);
	}

	char *pt;
	int number = strtol(argv[1], &pt, 16);
	printf("number = %x\n", number);
	printf("pt = %s\n", pt);
	return 0;
}

