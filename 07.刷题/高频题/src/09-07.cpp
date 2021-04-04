#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	string s1 = "acbcbcef";
	string s2 = "abcbced";

	int result[8 + 1][7 + 1] = { 0 };
	int flag = 0;
	int max = 0;
	for (int i = 1; i < 9; ++i)
	{
		for (int j = 1; j < 8; ++j)
		{
			if (s1[i - 1] == s2[j - 1])
			{
				result[i][j] = result[i - 1][j - 1] + 1;
				if (result[i][j] > max)
				{
					max = result[i][j];
					flag = j;
				}
			}
		}
	}

	cout << max << endl;
	cout << s1.substr(flag - max + 1, flag - 1) << endl;
	return 0;
}