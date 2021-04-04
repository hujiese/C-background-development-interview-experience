#include<iostream>
using namespace std;
struct B{
	char a;
	double b;
	int c;
}test_struct_b;
int main(){
	cout << sizeof(test_struct_b) << endl;
	return 0;
}
