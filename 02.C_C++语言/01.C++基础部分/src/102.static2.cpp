#include<iostream>
using namespace std;

void change()
{
	static int a = 0;
	++a;
	cout << a << endl;
}

int main(void)
{
	change(); // 1
	change(); // 2
	change(); // 3
	change(); // 4
	return 0;
}