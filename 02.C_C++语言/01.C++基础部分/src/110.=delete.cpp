#include <iostream>
using namespace std;

class DataOnly 
{
public:
	DataOnly() {}
	~DataOnly() {}

	DataOnly(const DataOnly & rhs) = delete; //禁止使用该函数
	DataOnly & operator=(const DataOnly & rhs) = delete; //禁止使用该函数

	DataOnly(const DataOnly && rhs) {}
	DataOnly & operator=(DataOnly && rhs) {}
};

int main(int argc, char *argv[])
{
	DataOnly data1;
	DataOnly data2(data1); // 	1	IntelliSense:  无法引用 函数 "DataOnly::DataOnly(const DataOnly &rhs)" (已声明 所在行数:9) -- 它是已删除的函数
	DataOnly data3 = data1; // 1	IntelliSense:  无法引用 函数 "DataOnly::DataOnly(const DataOnly &rhs)" (已声明 所在行数:9) -- 它是已删除的函数
	return 0;
}