#include<iostream>
using namespace std;
class CBox{
public:
	CBox(){};
	virtual ~CBox(){};
};
int main(){
	CBox boxobj;
	cout << sizeof(boxobj) << endl;//4
	return 0;
}
