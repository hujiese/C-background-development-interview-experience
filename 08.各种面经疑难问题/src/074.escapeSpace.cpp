#include <iostream>

using namespace std;

char* escapeSpace(char *str)
{
	int len = strlen(str);
	char* begin = str;
	char* end = str + len - 1;

	while (*begin == ' ')
		++begin;

	while (*end == ' ')
		*end-- = '\0';

	return begin;
}

int main(void)
{
	char str[] = "   abcdef     ";
	cout << str << endl;

	cout << escapeSpace(str) << endl;
}