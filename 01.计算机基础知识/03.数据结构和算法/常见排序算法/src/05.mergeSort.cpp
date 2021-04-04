#include <iostream>

using namespace std;

void mergeSort(int* data, int* reg, int start, int end)
{
	if (start >= end)
		return;

	int mid = (start + end) >> 1;
	int start1 = start;
	int end1 = mid;
	int start2 = mid + 1;
	int end2 = end;

	mergeSort(data, reg, start1, end1);
	mergeSort(data, reg, start2, end2);

	int k = start;
	while (start1 <= end1 && start2 <= end2)
		reg[k++] = data[start1] < data[start2] ? data[start1++] : data[start2++];

	while (start1 <= end1)
		reg[k++] = data[start1++];

	while (start2 <= end2)
		reg[k++] = data[start2++];

	for (int i = start; i <= end; ++i)
		data[i] = reg[i];
}

void mergeSort(int* array, int length)
{
	int* reg = new int[length];
	mergeSort(array, reg, 0, length - 1);
}

int main(void)
{
	int array[9] = { 56, 23, 0, 5, 3, 6, 2, 7, 12 };

	mergeSort(array, 9);
	for (int i : array)
		cout << i << " ";
	cout << endl;
	return 0;
}