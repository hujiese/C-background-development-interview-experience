#include <iostream>
using namespace std;

int getMax(int* array, int length)
{
	if (array == nullptr || length <= 0)
		return -1;

	int max = 0;
	int currentMax = 0;

	for (int i = 0; i < length; ++i)
	{
		if (currentMax < 0)
			currentMax = array[i];
		else
			currentMax += array[i];

		if (currentMax > max)
			max = currentMax;
	}
	return max;
}
int main(void)
{
	int array[8] = { 1, -2, 3, 10, -4, 7, 2, -5 };
	cout << getMax(array, 8) << endl;
	return 0;
}