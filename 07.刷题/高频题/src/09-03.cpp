#include <iostream>
using namespace std;

bool getSumNum(int* array, int length, int* num1, int* num2, int target)
{
	if (array == nullptr || length <= 0 || num1 == nullptr || num2 == nullptr)
		return false;

	int start = 0;
	int end = length - 1;

	while (start < end)
	{
		if (array[start] + array[end] == target)
		{
			*num1 = array[start];
			*num2 = array[end];
			return true;
		}
		else if (array[start] + array[end] > target)
			--end;
		else
			++start;
	}
	return false;
}
int main(void)
{
	int array[6] = { 1, 2, 4, 7, 11, 15 };
	int num1 = 0;
	int num2 = 0;
	if (getSumNum(array, 6, &num1, &num2, 15))
	{
		cout << num1 << " --- " << num2 << endl;
	}
	return 0;
}