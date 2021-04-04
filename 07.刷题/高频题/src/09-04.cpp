#include <iostream>
using namespace std;

int getMaxValue(int* array, int length)
{
	if (array == nullptr || length <= 2)
		return -1;

	int min = array[0];
	int maxValue = 0;
	int currentMaxVal = array[1] - min;
	for (int i = 2; i < length; ++i)
	{
		if (array[i - 1] < min)
			min = array[i - 1];

		currentMaxVal = array[i] - min;

		if (currentMaxVal > maxValue)
			maxValue = currentMaxVal;
	}

	return maxValue;
}
int main(void)
{
	int array[8] = { 9, 11, 8, 5, 7, 12, 16, 14 };
	cout << getMaxValue(array, 8) << endl;
	return 0;
}