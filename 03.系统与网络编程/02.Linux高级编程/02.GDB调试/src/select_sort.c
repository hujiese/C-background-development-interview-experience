#include <stdio.h>
#include <stdlib.h>

/*-------------------- 排序规则 --------------------

		它的工作原理是每一次从待排序的数据元素中选出
		最小（或最大）的一个元素，存放在序列的起始位
		置，直到全部待排序的数据元素排完。

		稳定性:选择排序是不稳定的排序方法 如:[5,5,3]

-------------------------------------------------*/

//选择排序(升序排列)
void selectionSort(int *array, int len)
{
	int min = 0;	// 指向最小的元素的位置
	// 外层循环
	for (int i = 0; i < len - 1; ++i)
	{
		min = i;
		// 内存循环
		for (int j = i + 1; j < len; ++j)
		{
			// 判断
			if (array[min] > array[j])
			{
				// 保存最小的元素的位置
				min = j;
			}
		}
		// 判断是否需要交换
		if (min != i)
		{
			// 找到了新的最小值
			// 交换
			int tmp = array[min];
			array[min] = array[i];
			array[i] = tmp;
		}
	}
}
