#include <iostream>
#include <vector>
using namespace std;

int main(void)
{
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);

	vector<int>::iterator it1 = vec.begin();
	++it1;
	cout << *it1 << "  size : " << vec.size() << "  capacity : "<< vec.capacity() << endl; // 2  size : 3  capacity : 3

	vec.push_back(4);
	vec.push_back(5);
	// push_back的扩容操作会导致迭代器失效
	cout << *it1 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 15648344  size : 5  capacity : 6

	cout << "-----------------------" << endl;

	vector<int>::iterator it2 = vec.begin();
	++it2;
	cout << *it2 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 5  capacity : 6
	vec.resize(4);
	cout << *it2 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 4  capacity : 6
	for (int i : vec)
		cout << i << " "; // 1 2 3 4
	cout << endl; // 使用resize时，如果输入新的长度比之前的要小，那么将发生截断，后面那部分元素将不存在

	cout << "-----------------------" << endl;

	vector<int>::iterator it3 = vec.begin();
	++it3;
	cout << *it3 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 4  capacity : 6
	vec.resize(5);
	cout << *it3 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 5  capacity : 6
	for (int i : vec)
		cout << i << " "; // 1 2 3 4 0
	cout << endl; // 使用resize时，如果输入新的长度比之前的要大，那么将添加新的数据并赋值为0
	//vec.resize(8);
	//cout << *it3 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 4745288  size : 8  capacity : 9

	cout << "-----------------------" << endl;

	vector<int>::iterator it4 = vec.begin();
	++it4;
	cout << *it4 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 5  capacity : 6
	vec.reserve(10); // reserve操作扩容将导致迭代器失效
	cout << *it4 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 17710248  size : 5  capacity : 10

	return 0;
}