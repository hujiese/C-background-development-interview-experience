#include <iostream>
using namespace std;

/*

�ο���https://www.cnblogs.com/lcchuguo/p/5210032.html

����������

��һ�����飬������������Ԫ�ص�ֵ����������0,1,2��Ȼ�����������������
�ڶ��ֱ����� ����n����������ֲ�ͬ��ɫ��С������������һ����ͨ��������������������
ʹ�ô������ң�������һЩ����һЩ����һЩ����

����˼·��

���ø�������partition��˼�룬������ָ�뽫������ɫ�������

�������ô˴����� July �������ϵ���к����������⡣

һ��ǰָ��begin��һ����ָ��current��һ����ָ��end��currentָ����������������У���

currentָ����ָԪ��Ϊ0ʱ����beginָ����ָ��Ԫ�ؽ���������current++��begin++ ��
currentָ����ָԪ��Ϊ1ʱ����������ʲô���������򲻶���������current++ ��
currentָ����ָԪ��Ϊ2ʱ����endָ����ָ��Ԫ�ؽ���������currentָ�벻����end-- ��

Ϊʲô������3���У�currentָ����ָԪ��Ϊ2ʱ����endָ����ָԪ�ؽ���֮��currentָ�벻�ܶ��أ�
���ڵ�������currentָ����ָԪ����endָ����ָԪ�ؽ���֮ǰ������endָ��֮ǰָ��Ԫ����0����ô��currentָ����ָԪ�ؽ���֮��
currentָ��˿���ָ��Ԫ����0����ʱ��currentָ���ܶ�ô�����ܶ���������������1��������
����currentָ����ָ��Ԫ����0��������beginָ����ָ��Ԫ�ؽ�����

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