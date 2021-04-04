#include <iostream>
using namespace std;

/*

参考：https://www.cnblogs.com/lcchuguo/p/5210032.html

问题描述：

给一个数组，而且数组里面元素的值仅仅可能是0,1,2，然后如今把这个数组排序。
第二种表述： 现有n个红白蓝三种不同颜色的小球，乱序排列在一起，请通过两两交换随意两个球，
使得从左至右，依次是一些红球、一些白球、一些蓝球。

解题思路：

采用高速排序partition的思想，用两个指针将三种颜色间隔开。

以下引用此处内容 July 编程艺术系列中荷兰国旗问题。

一个前指针begin，一个中指针current，一个后指针end，current指针遍历整个数组序列，当

current指针所指元素为0时，与begin指针所指的元素交换，而后current++，begin++ 。
current指针所指元素为1时，不做不论什么交换（即球不动），而后current++ 。
current指针所指元素为2时，与end指针所指的元素交换，而后，current指针不动，end-- 。

为什么上述第3点中，current指针所指元素为2时，与end指针所指元素交换之后，current指针不能动呢？
由于第三步中current指针所指元素与end指针所指元素交换之前，假设end指针之前指的元素是0，那么与current指针所指元素交换之后。
current指针此刻所指的元素是0，此时。current指针能动么？不能动，由于如上述第1点所述。
假设current指针所指的元素是0，还得与begin指针所指的元素交换。

*/
void solve(int* array, int length)
{
	int start = 0;
	int current = 0;
	int end = length - 1;

	while (current <= end)
	{
		if (array[current] == 0)
		{
			swap(array[current], array[start]);
			++start;
			++current;
		}
		else if (array[current] == 1)
			++current;
		else if (array[current] == 2)
		{
			swap(array[current], array[end]);
			--end;
		}
	}
}

int main(void)
{
	int array[8] = { 0, 2, 1, 0, 2, 1, 0, 1 };
	solve(array, 8);
	for (int i : array)
		cout << i << " ";
	cout << endl;
	return 0;
}