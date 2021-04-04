#include<iostream>
using namespace std;
class CBox{
	int length, width, height;
	static int count;
};
int main(){
	CBox boxobj;
	cout << sizeof(boxobj) << endl;
	return 0;
}
