#include <iostream>
#include <map>
#include <string>
using namespace std;

void test1(void)
{
	map<int, string> mp;
	mp.insert(pair<int, string>(1, "str1"));
	mp.insert(make_pair(2, "str2"));
	mp.insert(map<int, string>::value_type(3, "str3"));
	mp[3] = "str4";

	map<int, string>::iterator it = mp.find(34);
	if (it == mp.end())
	{
		cout << "we can't find the value to the key !" << endl;
	}
	else
	{
		cout << it->first << "\t" << it->second << endl;
	}
}

void test2(void)
{
	map<int, string> mp;
	mp.insert(pair<int, string>(1, "str1"));
	mp.insert(make_pair(2, "str2"));
	mp.insert(map<int, string>::value_type(3, "str3"));
	mp[3] = "str4";

	pair<map<int, string>::iterator, map<int, string>::iterator> pt = mp.equal_range(2);
	if (pt.first == mp.end())
	{
		cout << "we can't find the value to the key !" << endl;
	}
	else
	{
		cout << pt.first->first << "\t" << pt.first->second << endl;
	}

	if (pt.second == mp.end())
	{
		cout << "we can't find the value to the key !" << endl;
	}
	else
	{
		cout << pt.second->first << "\t" << pt.second->second << endl;
	}
}
int main(void)
{
	cout << "-----------test1-----------" << endl;
	test1();
	cout << "-----------test2-----------" << endl;
	test2();
	return 0;
}