#include <iostream>

using namespace std;

void BubbleSort(int* array, int length)
{
	for (int i = 0; i < length - 1; ++i)
	{
		for (int j = 0; j < length - i - 1; ++j)
		{
			if (array[j] > array[j + 1])
				swap(array[j], array[j + 1]);
		}
	}
}

int main(void)
{
	int array[9] = { 56, 23, 0, 5, 3, 6, 2, 7, 12 };
	BubbleSort(array, 9);
	for (int i : array)
		cout << i << " ";
	cout << endl;
	return 0;
}