#include <iostream>

using namespace std;

void insertSort(int* array, int length)
{
	for (int gap = length / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < length; ++i)
		{
			int temp = array[i];
			for (int j = i; j > 0; j -= gap)
			{
				if (array[j] < array[j - gap])
				{
					array[j] = array[j - gap];
					array[j - gap] = temp;
				}
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