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
	// push_back�����ݲ����ᵼ�µ�����ʧЧ
	cout << *it1 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 15648344  size : 5  capacity : 6

	cout << "-----------------------" << endl;

	vector<int>::iterator it2 = vec.begin();
	++it2;
	cout << *it2 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 5  capacity : 6
	vec.resize(4);
	cout << *it2 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 4  capacity : 6
	for (int i : vec)
		cout << i << " "; // 1 2 3 4
	cout << endl; // ʹ��resizeʱ����������µĳ��ȱ�֮ǰ��ҪС����ô�������ضϣ������ǲ���Ԫ�ؽ�������

	cout << "-----------------------" << endl;

	vector<int>::iterator it3 = vec.begin();
	++it3;
	cout << *it3 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 4  capacity : 6
	vec.resize(5);
	cout << *it3 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 5  capacity : 6
	for (int i : vec)
		cout << i << " "; // 1 2 3 4 0
	cout << endl; // ʹ��resizeʱ����������µĳ��ȱ�֮ǰ��Ҫ����ô������µ����ݲ���ֵΪ0
	//vec.resize(8);
	//cout << *it3 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 4745288  size : 8  capacity : 9

	cout << "-----------------------" << endl;

	vector<int>::iterator it4 = vec.begin();
	++it4;
	cout << *it4 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 2  size : 5  capacity : 6
	vec.reserve(10); // reserve�������ݽ����µ�����ʧЧ
	cout << *it4 << "  size : " << vec.size() << "  capacity : " << vec.capacity() << endl; // 17710248  size : 5  capacity : 10

	return 0;
}