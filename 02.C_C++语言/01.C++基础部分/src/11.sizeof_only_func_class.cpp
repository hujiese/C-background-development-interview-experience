#include<iostream>
using namespace std;
class CBox{
	int foo();
};
int main(){
	CBox boxobj;
	cout << sizeof(boxobj) << endl;
	return 0;
}
