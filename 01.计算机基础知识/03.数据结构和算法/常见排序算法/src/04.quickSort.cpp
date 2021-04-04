#include <iostream>

using namespace std;

void quickSort(int* array, int start, int end)
{
	if (start >= end)
		return;
	int left = start;
	int right = end;
	int key = array[start];

	while (left < right)
	{
		while (left < right && array[right] >= key)
			--right;
		if (left < right)
			// swap(array[left], array[right]);
			array[left] = array[right];

		while (left < right && array[left] <= key)
			++left;
		if (left < right)
			// swap(array[left], array[right]);
			array[right] = array[left];
	}
	array[left] = key;

	quickSort(array, start, left - 1);
	quickSort(array, left + 1, end);
}

int main(void)
{
	int array[9] = { 56, 23, 0, 5, 3, 6, 2, 7, 12 };
	quickSort(array, 0, 8);
	for (int i : array)
		cout << i << " ";
	cout << endl;
	return 0;
}