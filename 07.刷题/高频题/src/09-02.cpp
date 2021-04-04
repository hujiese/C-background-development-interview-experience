#include <iostream>
#include <string>
using namespace std;

int getMaxLen(string& str, int length, int* flag)
{
	int array[26] = { -1 };
	int maxLen = 0;
	int currentMaxLen = 0;
	for (int i = 0; i < length; ++i)
	{
		int index = str[i] - 'a';
		if (array[index] == -1 || currentMaxLen < (i - array[index]))
		{
			++currentMaxLen;
		}
		else
		{
			currentMaxLen = i - array[index];
		}
		array[index] = i;

		if (currentMaxLen > maxLen)
		{
			maxLen = currentMaxLen;
			*flag = i;
		}
	}
	return maxLen;
}

int main(void)
{
	string str = "arabcacfr";
	int flag = 0;
	int maxLen = getMaxLen(str, 9, &flag);
	cout << maxLen << endl;
	cout << str.substr(flag - maxLen + 1, maxLen) << endl;
	return 0;
}