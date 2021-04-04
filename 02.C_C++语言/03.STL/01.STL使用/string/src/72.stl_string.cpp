#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

//通过for 遍历string
void stringTest(void)
{
	string str = "hello";
	for (unsigned int i = 0; i < str.length(); i++)
	{
		cout << str[i] << " ";
	}
	cout << endl;
}
//通过迭代器遍历字符串
void stringTest2(void)
{
	string str = "hello";
	for (string::iterator it = str.begin(); it != str.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

//通过at()遍历字符串
void stringTest3(void)
{
	string str = "hello";

	try
	{
		for (unsigned int i = 0; i < str.length() + 3; i++)
		{
			cout << str.at(i) << " ";//此方法抛出异常
		}
		cout << endl;
	}
	catch (...)
	{
		cout << "exception" << endl;
	}
}

// 与 char * 的转换
void stringTest4(void)
{
	string str = "aaaabbbb";
	// string -->  char
	printf("%s\n", str.c_str());

	char buff[128] = { 0 };
	str.copy(buff, 3, 0);
	cout << buff << endl;
}

//字符串拼接
void stringTest5(void)
{
	//通过 '+'
	string str1 = "hel";
	string str2 = "lo !";
	str1 += str2;
	cout << str1 << endl;

	//通过 append()
	string str3 = "hel";
	string str4 = "lo !";
	str3 = str3.append(str4);
	cout << str3 << endl;
}

void stringTest6(void)
{
	string s2 = "AAAbbb";
	transform(s2.begin(), s2.end(), s2.begin(), toupper);
	cout << s2 << endl;

	string s3 = "AAAbbb";
	transform(s3.begin(), s3.end(), s3.begin(), tolower);
	cout << s3 << endl;

}

int main(void)
{
	//通过构造方法
	string s1 = "abcd";
	string s2("hello");
	string s3(5, 'a');

	cout << s1 << endl;
	cout << s2 << endl;
	cout << s3 << endl;

	cout << "----------1--------" << endl;
	stringTest();

	cout << "----------2--------" << endl;
	stringTest2();

	cout << "----------3--------" << endl;
	stringTest3();

	cout << "----------4--------" << endl;
	stringTest4();

	cout << "----------5--------" << endl;
	stringTest5();

	cout << "----------6--------" << endl;
	stringTest6();
	return 0;
}