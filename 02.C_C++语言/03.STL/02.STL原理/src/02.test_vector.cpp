#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
#include <algorithm> 	//sort()
using namespace std;

namespace jj02
{
	const long ASIZE = 50000L;//��������ַ�������

	//�û�������Ҫ���ҵ�string����Ԫ��
	string get_a_target_string()
	{
		long target = 0;
		char buf[10];

		cout << "target (0~" << RAND_MAX << "): ";
		cin >> target;
		sprintf(buf, "%d", target);
		return string(buf);
	}

	//�ַ����Ƚ�
	int compareStrings(const void* a, const void* b)
	{
		if (*(string*)a > *(string*)b)
			return 1;
		else if (*(string*)a < *(string*)b)
			return -1;
		else
			return 0;
	}

	void test_vector(const long& value)
	{
		cout << "\ntest_vector().......... \n";

		vector<string> c;//����һ��vector�������string��������
		char buf[10];

		clock_t timeStart = clock();//��ʼ��ʱ
		//��value��string����vector��
		for (long i = 0; i< value; ++i)
		{
			try {//����ʹ��try...catch��Ϊ�˱���value̫�����ڴ治�������쳣
				sprintf(buf, "%d", rand());
				c.push_back(string(buf));//��string����vector��
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				//������� i=58389486 then std::bad_alloc
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;//��ӡ��ʼ��arrayʱ��
		cout << "vector.max_size()= " << c.max_size() << endl;	//1073747823
		cout << "vector.size()= " << c.size() << endl; // ��ӡvector����
		cout << "vector.front()= " << c.front() << endl;//��ӡvector��Ԫ��
		cout << "vector.back()= " << c.back() << endl;//��ӡvector���Ԫ��
		cout << "vector.data()= " << c.data() << endl;//��ӡvector���ڴ���׵�ַ
		cout << "vector.capacity()= " << c.capacity() << endl << endl;//��ӡvector����


		string target = get_a_target_string();//��ȡ�û������ַ���
		//�ô�������stl��find�����������û������ַ���������ӡ����������ʱ��
		{
			timeStart = clock();
			auto pItem = find(c.begin(), c.end(), target);
			cout << "std::find(), milli-seconds : " << (clock() - timeStart) << endl;

			if (pItem != c.end())
				cout << "found, " << *pItem << endl << endl;
			else
				cout << "not found! " << endl << endl;
		}
		//�ô�������c��׼��Ķ��ֲ���bsearch�����������û������ַ���������ӡ����������ʱ��
		{
			timeStart = clock();
			sort(c.begin(), c.end());//����
			cout << "sort(), milli-seconds : " << (clock() - timeStart) << endl;//��ӡ����ʱ��

			timeStart = clock();
			string* pItem = (string*)::bsearch(&target, (c.data()),//���ֲ���
				c.size(), sizeof(string), compareStrings);
			cout << "bsearch(), milli-seconds : " << (clock() - timeStart) << endl;//��ӡ���ֲ���ʱ��

			if (pItem != NULL)
				cout << "found, " << *pItem << endl << endl;
			else
				cout << "not found! " << endl << endl;
		}

	}
}

int main(void)
{
	jj02::test_vector(jj02::ASIZE);
	return 0;
}