#include <iostream>

using namespace std;

void insertSort(int* array, int length)
{
	for (int i = 1; i < length; ++i)
	{
		int temp = array[i];
		for (int j = i + 1; j > 0; --j)
		{
			if (array[j] < array[j - 1])
			{
				array[j] = array[j - 1];
				array[j - 1] = temp;
			}
		}
	}
}

int main(void)
{
	int array[9] = { 56, 23, 0, 5, 3, 6, 2, 7, 12 };
	insertSort(array, 9);
	for (int i : array)
		cout << i << " ";
	cout << endl;
	return 0;
}