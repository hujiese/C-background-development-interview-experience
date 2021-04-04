#include<iostream>
using namespace std;
class  counter
{
private:
	static  int  num; //声明与定义静态数据成员
public:
	void  setnum(int i) { num = i; }	 //成员函数访问静态数据成员
	void  shownum() { cout << num << endl; }
	void  setnum2(int i) { num2 = i; }	 //成员函数访问静态数据成员
	void  shownum2() { cout << num2 << endl; }
	static int num2;
};
int  counter::num = 0;//声明与定义静态数据成员
int  counter::num2 = 0;

void main()
{
	counter  a, b;
	cout << "---------test private static----------" << endl;
	//调用成员函数访问私有静态数据成员
	a.shownum(); // 0
	b.shownum(); // 0
	a.setnum(10);
	a.shownum();// 10
	b.shownum();// 10

	cout << "---------test public static----------" << endl;
	// counter::num = 5; // error ! 私有static成员变量不可如此访问
	counter::num2 = 12;
	a.shownum2(); // 12
	b.shownum2(); // 12
	b.setnum2(18);
	a.shownum2(); // 18
	b.shownum2(); // 18
	a.num2 = 23;
	a.shownum2(); // 23
	b.shownum2(); // 23
}
