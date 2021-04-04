#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int rand7(void)
{
	srand((int)time(0));
	int num = rand() % 7 + 1;
	return num;
}

int getBit(void)
{
	int num = 0;
	while ((num = rand7()) == 7);

	return num & 1;
}

int getNum()
{
	int num;
	do
	{
		num = 0;
		for (int i = 0; i < 4; ++i)
		{
			num = num << 1 | getBit();
		}
	} while (num <= 0 || num > 10);

	return num;
}

int main(void)
{
	for (int i = 0; i < 50;++i)
		cout << getNum() << endl;

	return 0;
}