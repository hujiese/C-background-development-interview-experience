#include <stdio.h>
#include <stdlib.h>

/******************* 排序规则 *******************

	每次处理就是将无序数列的第一个元素与有序数列
	的元素从后往前逐个进行比较，找出插入位置，将
	该元素插入到有序数列的合适位置中。

	稳定性:插入排序是稳定的

***********************************************/

//插入排序算法(升序排列)
void insertionSort(int *array, int len)
{
	int tmp = 0;	// 存储基准数
	int index = 0;	// 坑的位置
	// 遍历无序序列
	for (int i = 1; i < len; ++i)
	{
		index = i;
		tmp = array[i];
		// 遍历有序序列(从后往前)
		for (int j = i - 1; j >= 0; --j)
		{
			// 基准数根有序序列中的元素比较
			if (tmp < array[j])
			{
				// 有序序列元素后移
				array[j + 1] = array[j];
				// 坑的位置
				index = j;
			}
			else
			{
				break;
			}
		}
		// 填坑
		array[index] = tmp;
	}
}

