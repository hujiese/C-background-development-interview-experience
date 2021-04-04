#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	string str = "PATZJUJZTACCBCC";

	int result[15][15] = { 0 };
	for (int i = 0; i < 15; ++i)
		result[i][i] = 1;

	int max = 1;
	int flag = 0;
	for (int L = 3; L < 15; ++L)
	{
		for (int i = 0; i + L - 1 < 15; ++i)
		{
			int j = i + L - 1;
			if (str[i] == str[j] && result[i + 1][j - 1] == 1)
			{
				result[i][j] = 1;
				max = L;
				flag = i;
			}
		}
	}

	cout << max << endl;
	cout << str.substr(flag, flag + max - 1) << endl;

	return 0;
}