#include<iostream>
using namespace std;
class CBox{
public:
	CBox(){};
	~CBox(){};
};
int main(){
	CBox boxobj;
	cout << sizeof(boxobj) << endl;
	return 0;
}
