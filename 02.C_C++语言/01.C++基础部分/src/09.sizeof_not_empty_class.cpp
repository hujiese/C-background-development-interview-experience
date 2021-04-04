#include<iostream>
using namespace std;
class CBox{
	int length, width, height;
};
int main(){
	CBox boxobj;
	cout << sizeof(boxobj) << endl;
	return 0;
}
