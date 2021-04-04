#include <iostream>
using namespace std;

int getCutNums(int length)
{
	if (length < 2)
		return 0;
	if (length == 2)
		return 1;
	if (length == 3)
		return 2;

	int* result = new int[length + 1];
	result[0] = 0;
	result[1] = 1;
	result[2] = 2;
	result[3] = 3;

	int max = 0;
	for (int i = 4; i <= length; ++i)
	{
		max = 0;
		for(int j = 1; j <= i / 2; ++j)
		{
			int temp = result[j] * result[i - j];
			if (max < temp)
				max = temp;

			result[i] = max;
		}
	}
	max = result[length];
	delete[] result;

	return max;
}

int main(void)
{
	cout << getCutNums(5) << endl;
	return 0;
}