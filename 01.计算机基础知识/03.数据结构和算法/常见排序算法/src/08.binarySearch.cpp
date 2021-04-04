#include <iostream>

using namespace std;

int binarySearch(int* array, int target, int length)
{
	int start = 0;
	int end = length - 1;

	while (start <= end)
	{
		int mid = (start + end) >> 1;

		if (target == array[mid])
			return mid;
		else if (target > array[mid])
			start = mid + 1;
		else
			end = mid - 1;
	}
	return -1;
}

int main(void)
{
	int array[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	cout << binarySearch(array, 9, 9) << endl;
	return 0;
}