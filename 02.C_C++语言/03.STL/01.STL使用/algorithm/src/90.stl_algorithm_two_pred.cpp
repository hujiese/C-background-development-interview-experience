#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

void current(int &v)
{
	cout << v << " ";
}

bool MyCompare(const int &a, const int &b)
{
	return a < b;
}
void main()
{
	vector<int> v(10);

	for (int i = 0; i<10; i++)
	{
		v[i] = rand() % 100;
	}

	for_each(v.begin(), v.end(), current);
	printf("\n");
	sort(v.begin(), v.end(), MyCompare);

	printf("\n");
	for (int i = 0; i<10; i++)
	{
		printf("%d ", v[i]);
	}
	printf("\n");
}
