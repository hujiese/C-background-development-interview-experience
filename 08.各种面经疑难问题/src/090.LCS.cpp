#include <iostream>
#include <algorithm>
using namespace std;

int main(void)
{
	int array1[8] = { 1, 3, 4, 5, 6, 7, 7, 8 };
	int array2[9] = { 3, 5, 7, 4, 8, 6, 7, 8, 2 };

	int result[8 + 1][9 + 1] = { 0 };
	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			if (array1[i - 1] == array2[j - 1])
				result[i][j] =result[i - 1][j - 1] + 1;
			else
				result[i][j] = max(result[i][j - 1], result[i - 1][j]);
		}
	}
	cout << result[8][9] << endl; // 5
	return 0;
}