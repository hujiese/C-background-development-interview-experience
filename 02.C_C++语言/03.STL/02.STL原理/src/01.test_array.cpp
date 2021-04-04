#define _CRT_SECURE_NO_WARNINGS
#include <array>
#include <iostream>
#include <ctime> 
#include <cstdlib> //qsort, bsearch, NULL
using namespace std;

namespace jj01
{
	const long ASIZE = 50000L;

	//用户输入需要查找的long类型元素
	long get_a_target_long()
	{
		long target = 0;

		cout << "target (0~" << RAND_MAX << "): ";
		cin >> target;
		return target;
	}

	//比较传入参数大小，a > b则返回1，否则返回-1
	int compareLongs(const void* a, const void* b)
	{
		return (*(long*)a - *(long*)b);
	}

	void test_array()
	{
		cout << "\ntest_array().......... \n";

		array<long, ASIZE> c;//50000个long类型元素

		clock_t timeStart = clock();//开始计时
		for (long i = 0; i< ASIZE; ++i) {//为array随机生成50000个long类型元素
			c[i] = rand();
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;	//打印初始化array时间
		cout << "array.size()= " << c.size() << endl;//打印数组长度
		cout << "array.front()= " << c.front() << endl;//打印数组首元素
		cout << "array.back()= " << c.back() << endl;//打印数组最后元素
		cout << "array.data()= " << c.data() << endl;//打印数组在内存的首地址

		long target = get_a_target_long();

		timeStart = clock();//排序查找开始计时
		::qsort(c.data(), ASIZE, sizeof(long), compareLongs);//排序
		long* pItem = (long*)::bsearch(&target, (c.data()), ASIZE, sizeof(long), compareLongs);//二分法查找
		cout << "qsort()+bsearch(), milli-seconds : " << (clock() - timeStart) << endl;	// 打印排序查找时间
		if (pItem != NULL)//判断是否找到
			cout << "found, " << *pItem << endl;
		else
			cout << "not found! " << endl;
	}
}

int main(void)
{
	jj01::test_array();
	return 0;
}