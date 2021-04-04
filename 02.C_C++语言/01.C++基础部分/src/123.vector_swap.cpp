#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
	vector<int> vec;

	vec.push_back(1);
	cout << vec.size() << "----" << vec.capacity() << endl; // 1----1

	vec.push_back(2);
	cout << vec.size() << "----" << vec.capacity() << endl; // 2----2

	vec.push_back(3);
	cout << vec.size() << "----" << vec.capacity() << endl; // 3----3

	vec.push_back(4);
	cout << vec.size() << "----" << vec.capacity() << endl; // 4----4

	vec.push_back(5);
	cout << vec.size() << "----" << vec.capacity() << endl; // 5----6

	vec.push_back(6);
	cout << vec.size() << "----" << vec.capacity() << endl; // 6----6

	vec.push_back(7);
	cout << vec.size() << "----" << vec.capacity() << endl; // 7----9

	vec.reserve(16);
	cout << vec.size() << "----" << vec.capacity() << endl; // 7----16

	//vector<int>().swap(vec); // ÊÍ·ÅvectorÄÚ´æ
	//cout << vec.size() << "----" << vec.capacity() << endl; // 0----0

	vector<int>(vec).swap(vec);
	cout << vec.size() << "----" << vec.capacity() << endl; // 7----7
	for (int i : vec)
		cout << i << " "; // 1 2 3 4 5 6 7
	cout << endl;
}
