#include <iostream>
#include "string"
#include "algorithm"

using namespace std;

//find 函数查找字符串，返回从索引出的第一个下标
void test1(void)
{
	string str = "abc hello abc hello";
	int index = str.find("hello", 0);
	cout << index << endl;
}

//连续获取目标字符串下标
void test2(void)
{
	string str = "aad hello aag hello aga hello aga hello";
	int offindex = str.find("hello", 0);

	while (offindex != string::npos)
	{
		cout << offindex << endl;
		offindex = offindex + 1;
		offindex = str.find("hello", offindex);
	}

}

//用replace将目标字符串改为大写
void test3(void)
{
	string str = "aad hello aag hello aga hello aga hello";
	int offindex = str.find("hello", 0);

	while (offindex != string::npos)
	{
		str.replace(offindex, 5, "HELLO");
		offindex = offindex + 1;
		offindex = str.find("hello", offindex);
		cout << str << endl;
	}

}
//用erase方法删除指定字符
void test4(void)
{
	string str = "hello hello llo llo ll o";
	string::iterator it = find(str.begin(), str.end(), 'l');

	while (it != str.end())
	{
		str.erase(it);
		cout << str << endl;
	}
}

//用erase方法删除全部字符串
void test5(void)
{
	string str = "hello hello llo llo ll o";
	str.erase(str.begin(), str.end());
	cout << str << endl;
	cout << str.length() << endl;
}

//用insert方法在指点位置插入字符串
void test6(void)
{
	string str = "hello hello";
	str.insert(0, "world");//不可以传递str.begin()等;
	cout << str << endl;
}
//用transform与toupper/tolower方法转换指定字符串的大小写
void test7(void)
{
	string str = "hello";
	transform(str.begin(), str.end(), str.begin(), toupper);
	cout << str << endl;
}
int main(void)
{
	cout << "-----------test1----------" << endl;
	test1();
	cout << "-----------test2----------" << endl;
	test2();
	cout << "-----------test3----------" << endl;
	test3();
	cout << "-----------test4----------" << endl;
	test4();
	cout << "-----------test5----------" << endl;
	test5();
	cout << "-----------test6----------" << endl;
	test6();
	cout << "-----------test7----------" << endl;
	test7();
	return 0;
}