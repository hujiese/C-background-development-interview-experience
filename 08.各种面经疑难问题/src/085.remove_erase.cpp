#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(void)
{
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	vec.push_back(6);
	vec.push_back(7);

	remove(vec.begin(), vec.end(), 3);

	for (int i : vec)
		cout << i << " ";
	cout << endl; // 1 2 4 5 6 7 6 7
	return 0;
}