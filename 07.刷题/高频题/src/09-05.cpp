#include <iostream>
#include <algorithm>
using namespace std;

int getLIS(int* array, int length)
{
	if (array == nullptr || length <= 0)
		return -1;

	int* result = new int[length];
	for (int i = 0; i < length; ++i)
		result[i] = 1;

	for (int i = 1; i < length; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (array[i] > array[j])
				result[i] = max(result[i], result[j] + 1);
		}
	}

	int len = 0;
	for (int i = 0; i < length; ++i)
	{
		if (result[i] > len)
			len = result[i];
	}

	delete[] result;
	return len;
}

int getLISEffect(int* array, int length)
{
	if (array == nullptr || length <= 0)
		return -1;

	int* temp = new int[length];
	int* result = new int[length];

	for (int i = 0; i < length; ++i)
		temp[i] = 0x7fffffff;

	int max = 0;
	for (int i = 0; i < length; ++i)
	{
		int j = lower_bound(temp, temp + length, array[i]) - temp;
		result[i] = j + 1;
		if (result[i] > max)
			max = result[i];

		temp[j] = array[i];
	}

	delete[] temp;
	delete[] result;
	return max;
}

int main(void)
{
	int array[9] = { 2, 7, 1, 5, 6, 4, 3, 8, 9 };
	//cout << getLIS(array, 9) << endl;
	cout << getLISEffect(array, 9) << endl;
	return 0;
}