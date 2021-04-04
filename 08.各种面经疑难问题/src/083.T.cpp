#include <iostream>
#include <vector>
using namespace std;

class T1
{
public:
	T1() = delete;
};

class T2
{
private:
	T2();
};

int main(void)
{
	vector<T1> vec;
	T1 array[10]; // 	无法引用 "T1" 的默认构造函数 -- 它是已删除的函数

	vector<T2> vec;
	T2 array[10]; // 	"T2::T2()" (已声明 所在行数:14) 不可访问

	return 0;
}