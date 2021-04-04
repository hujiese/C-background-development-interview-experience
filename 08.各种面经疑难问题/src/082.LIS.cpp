#include <iostream>
#include <algorithm>
using namespace std;

int getLIS(int* array, int length)
{
	int* LIS = new int[length];

	for (int i = 0; i < length; ++i)
		LIS[i] = 1;

	for (int i = 1; i < length; ++i)
	{
		for (int j = 1; j < i; ++j)
		{
			if (array[j] < array[i])
				LIS[i] = max(LIS[i], LIS[j] + 1);
		}
	}

	int result = 0;
	for (int i = 0; i < length; ++i)
		result = max(result, LIS[i]);

	delete[] LIS;

	return result;
}

int getLISEffect(int* array, int length)
{
	int* LIS = new int[length];
	int* temp = new int[length];

	for (int i = 0; i < length; ++i)
		temp[i] = 0x7fffffff;

	int result = -1;
	for (int i = 0; i < length; ++i)
	{
		int j = lower_bound(temp, temp + length, array[i]) - temp;
		LIS[i] = j + 1;

		if (result < LIS[i])
			result = LIS[i];

		temp[j] = array[i];
	}

	delete[] LIS;
	delete[] temp;

	return result;
}

int main(void)
{
	int array[9] = { 2, 7, 1, 5, 6, 4, 3, 8, 9 };
	/*cout << getLIS(array, 9) << endl;*/
	cout << getLISEffect(array, 9) << endl;
	return 0;
}

