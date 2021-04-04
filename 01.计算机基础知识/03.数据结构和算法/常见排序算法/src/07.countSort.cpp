#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ¼ÆÊýÅÅÐò
void CountSort(int* vecRaw, int* vecObj, int length)
{
	int max = vecRaw[0];
	for (int i = 1; i < length; ++i)
	{
		if (vecRaw[i] > max)
			max = vecRaw[i];
	}

	int* arry = new int[max + 1];
	for (int k = 0; k <= max; ++k)
		arry[k] = 0;

	for (int i = 0; i < length; ++i)
		++arry[vecRaw[i]];

	for (int j = 1; j <= max; ++j)
		arry[j] += arry[j - 1];

	for (int i = length - 1; i >= 0; --i)
	{
		vecObj[--arry[vecRaw[i]]] = vecRaw[i];
	}

	delete[] arry;
}

int main()
{
	int vecRaw[14] = { 0, 5, 7, 9, 6, 3, 4, 5, 2, 8, 6, 9, 2, 1 };
	int vecObj[14] = { 0 };
	CountSort(vecRaw, vecObj, 14);
	for (int i = 0; i < 14; ++i)
		cout << vecObj[i] << "  ";
	cout << endl;

	return 0;
}