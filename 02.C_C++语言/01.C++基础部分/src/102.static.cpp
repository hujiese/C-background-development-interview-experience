#include<iostream>
using namespace std;
class  counter
{
private:
	static  int  num; //�����붨�徲̬���ݳ�Ա
public:
	void  setnum(int i) { num = i; }	 //��Ա�������ʾ�̬���ݳ�Ա
	void  shownum() { cout << num << endl; }
	void  setnum2(int i) { num2 = i; }	 //��Ա�������ʾ�̬���ݳ�Ա
	void  shownum2() { cout << num2 << endl; }
	static int num2;
};
int  counter::num = 0;//�����붨�徲̬���ݳ�Ա
int  counter::num2 = 0;

void main()
{
	counter  a, b;
	cout << "---------test private static----------" << endl;
	//���ó�Ա��������˽�о�̬���ݳ�Ա
	a.shownum(); // 0
	b.shownum(); // 0
	a.setnum(10);
	a.shownum();// 10
	b.shownum();// 10

	cout << "---------test public static----------" << endl;
	// counter::num = 5; // error ! ˽��static��Ա����������˷���
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
