#include<iostream>
using namespace std;
int main(){
	enum weather{ sunny, cloudy, rainy, windy };
	/*����sunny=0,cloudy=1,rainy=2,windy=3,
	Ĭ�ϵأ���һ��ö���ӱ���ֵΪ0*/
	enum fruits{ apple = 3, orange, banana = 7, bear };
	/*Ҳ������ʽ�ظ�ֵ����������ö����ȡֵ��ǰ��һ��ö���ӵ�ȡֵ+1����orange=4��bear=8*/
	cout << orange << endl;
	enum big_cities{ guangzhou = 1, shenzhen = 3, beijing = 1, shanghai = 2 };
	/*ͬһö���е�ö���ӵ�ȡֵ����ҪΨһ*/
	return 0;
}
