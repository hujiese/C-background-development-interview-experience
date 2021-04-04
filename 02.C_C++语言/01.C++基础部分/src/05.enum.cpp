#include<iostream>
using namespace std;
int main(){
	enum weather{ sunny, cloudy, rainy, windy };
	/*其中sunny=0,cloudy=1,rainy=2,windy=3,
	默认地，第一个枚举子被赋值为0*/
	enum fruits{ apple = 3, orange, banana = 7, bear };
	/*也可以显式地赋值，接下来的枚举子取值是前面一个枚举子的取值+1，即orange=4，bear=8*/
	cout << orange << endl;
	enum big_cities{ guangzhou = 1, shenzhen = 3, beijing = 1, shanghai = 2 };
	/*同一枚举中的枚举子的取值不需要唯一*/
	return 0;
}
