#define _CRT_SECURE_NO_WARNINGS
#include <array>
#include <iostream>
#include <ctime> 
#include <cstdlib> //qsort, bsearch, NULL
using namespace std;

namespace jj01
{
	const long ASIZE = 50000L;

	//�û�������Ҫ���ҵ�long����Ԫ��
	long get_a_target_long()
	{
		long target = 0;

		cout << "target (0~" << RAND_MAX << "): ";
		cin >> target;
		return target;
	}

	//�Ƚϴ��������С��a > b�򷵻�1�����򷵻�-1
	int compareLongs(const void* a, const void* b)
	{
		return (*(long*)a - *(long*)b);
	}

	void test_array()
	{
		cout << "\ntest_array().......... \n";

		array<long, ASIZE> c;//50000��long����Ԫ��

		clock_t timeStart = clock();//��ʼ��ʱ
		for (long i = 0; i< ASIZE; ++i) {//Ϊarray�������50000��long����Ԫ��
			c[i] = rand();
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;	//��ӡ��ʼ��arrayʱ��
		cout << "array.size()= " << c.size() << endl;//��ӡ���鳤��
		cout << "array.front()= " << c.front() << endl;//��ӡ������Ԫ��
		cout << "array.back()= " << c.back() << endl;//��ӡ�������Ԫ��
		cout << "array.data()= " << c.data() << endl;//��ӡ�������ڴ���׵�ַ

		long target = get_a_target_long();

		timeStart = clock();//������ҿ�ʼ��ʱ
		::qsort(c.data(), ASIZE, sizeof(long), compareLongs);//����
		long* pItem = (long*)::bsearch(&target, (c.data()), ASIZE, sizeof(long), compareLongs);//���ַ�����
		cout << "qsort()+bsearch(), milli-seconds : " << (clock() - timeStart) << endl;	// ��ӡ�������ʱ��
		if (pItem != NULL)//�ж��Ƿ��ҵ�
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