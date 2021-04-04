#include <iostream>
using namespace std;

class DataOnly 
{
public:
	void fun(int a) {}
	void fun(float a) = delete;
};

int main(int argc, char *argv[]) 
{
	DataOnly data1;
	data1.fun(1); // OK
	data1.fun(0.5); // 1	IntelliSense:  有多个 重载函数 "DataOnly::fun" 实例与参数列表匹配
	return 0;
}

