#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	string s1 = "acbcbcef";
	string s2 = "abcbced";

	int maxLen = 0;
	int flag = 0;
	int array[9][8] = { 0 };
	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 1; j <= 7; ++j)
		{
			if (s1[i - 1] == s2[j - 1])
				array[i][j] = array[i - 1][j - 1] + 1;

			if (array[i][j] > maxLen)
			{
				maxLen = array[i][j];
				flag = j;
			}
		}
	}

	cout << "最大字串长度为 : " << maxLen << endl; // 5
	cout << "最大字串为 : " << s1.substr(flag - maxLen + 1, flag - 1) << endl; // bcbce

}