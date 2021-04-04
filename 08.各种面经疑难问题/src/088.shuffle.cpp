#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;

void shuffle(int* array, int length)
{
	if (array == nullptr || length <= 0)
		return;

	srand((int)time(0));

	for (int i = 0; i < length; ++i)
	{
		int index = rand() % length; // 全局洗牌
		// int index = i + rand() % (length - i); // 局部洗牌
		
		int temp = array[i];
		array[i] = array[index];
		array[index] = temp;
	}
}

// 采用c++ random_shuffle洗牌算法
void shuffle2(int* array, int length)
{
	if (array == nullptr || length <= 0)
		return;

	random_shuffle(array, array + length);
}
int main(void)
{
	int array[6] = { 1, 2, 3, 4, 5, 6 };

	for (int i = 0; i < 10; ++i)
	{
		shuffle(array, 6);
		//shuffle2(array, 6);
		for (int i : array)
			cout << i << " ";
		cout << endl;
	}
	return 0;
}