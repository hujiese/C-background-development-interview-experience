#include <iostream>

using namespace std;

void selectSort(int* array, int length)
{
	for (int i = 0; i < length - 1; ++i)
	{
		int min = i;
		for (int j = i + 1; j < length; ++j)
		{
			if (array[min] > array[j])
				min = j;
		}
		if (min != i)
			swap(array[min], array[i]);
	}
}

int main(void)
{
	int array[9] = { 56, 23, 0, 5, 3, 6, 2, 7, 12 };
	selectSort(array, 9);
	for (int i : array)
		cout << i << " ";
	cout << endl;
	return 0;
}