#include <iostream>
using namespace std;

class DataOnly 
{
public:
	DataOnly() {}
	~DataOnly() {}

	DataOnly(const DataOnly & rhs) = delete; //��ֹʹ�øú���
	DataOnly & operator=(const DataOnly & rhs) = delete; //��ֹʹ�øú���

	DataOnly(const DataOnly && rhs) {}
	DataOnly & operator=(DataOnly && rhs) {}
};

int main(int argc, char *argv[])
{
	DataOnly data1;
	DataOnly data2(data1); // 	1	IntelliSense:  �޷����� ���� "DataOnly::DataOnly(const DataOnly &rhs)" (������ ��������:9) -- ������ɾ���ĺ���
	DataOnly data3 = data1; // 1	IntelliSense:  �޷����� ���� "DataOnly::DataOnly(const DataOnly &rhs)" (������ ��������:9) -- ������ɾ���ĺ���
	return 0;
}