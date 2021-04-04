#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

void main()
{
	vector<string> v1;
	v1.push_back("bbb");
	v1.push_back("aaa");
	v1.push_back("ccc");
	v1.push_back("zzzz");
	v1.push_back("ccc");
	string s1 = "ccc";
	//int num = count_if(v1.begin(),v1.end(), equal_to<string>(),s1);
	int num = count_if(v1.begin(), v1.end(), bind2nd(equal_to<string>(), s1));
	cout << num << endl;
}
