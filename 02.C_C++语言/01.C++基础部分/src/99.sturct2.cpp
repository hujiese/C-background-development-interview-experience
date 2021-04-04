#include <iostream>
using namespace std;

struct F
{
	char a;
	short b;
	double c;
	float d;
	char e;
};


#pragma back(push)
#pragma pack(2) // 2 bytes¶ÔÆë
struct E
{
	char a;
	short b;
	double c;
	float d;
	char e;
};
#pragma back(pop)


int main(void)
{
	int FCount = sizeof(F);
	cout << "F count = " << FCount << endl;  //F count = 24

	int ECount = sizeof(E);
	cout << "E count = " << ECount << endl;  //E count = 18

	return 0;
}