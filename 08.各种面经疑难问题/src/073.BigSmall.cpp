#include <iostream>

using namespace std;

int main(void)
{
	int num = 0x12345678;

	if (*((char *)&num) == 0x12)
		cout << "Big..." << endl;
	else
		cout << "Small.." << endl; // 打印改行

	char* p1 = (char *)&num;
	char* p2 = p1 + 1;
	char* p3 = p2 + 1;
	char* p4 = p3 + 1;

	char temp = *p1;
	*p1 = *p4;
	*p4 = temp;

	temp = *p2;
	*p2 = *p3;
	*p3 = temp;

	if (*((char *)&num) == 0x12)
		cout << "Big..." << endl; // 打印改行
	else
		cout << "Small.." << endl;
}