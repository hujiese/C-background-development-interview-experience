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

	mp[4] = "str4";


	for (map<int, string>::iterator it = mp.begin(); it != mp.end(); it++)
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

	mp[4] = "str4";

	map<int, string>::iterator it;
	while (!mp.empty())
	{
		it = mp.begin();
		mp.erase(it);
	}
	cout << " size : " << mp.size() << endl;
}

void test3(void)
{
	map<int, string> map1;

	//typedef pair<iterator, bool> _Pairib;

	//方法1
	pair<map<int, string>::iterator, bool>  mypair1 = map1.insert(pair<int, string>(1, "teacher01"));
	map1.insert(pair<int, string>(2, "teacher02"));

	//方法2 
	pair<map<int, string>::iterator, bool>  mypair3 = map1.insert(make_pair(3, "teacher04"));
	map1.insert(make_pair(4, "teacher05"));

	//方法3 
	pair<map<int, string>::iterator, bool>  mypair5 = map1.insert(map<int, string>::value_type(5, "teacher05"));
	if (mypair5.second != true)
	{
		cout << "key 5 插入失败" << endl;
	}
	else
	{
		cout << mypair5.first->first << "\t" << mypair5.first->second << endl;
	}
	// 插入失败
	pair<map<int, string>::iterator, bool>  mypair6 = map1.insert(map<int, string>::value_type(5, "teacher55"));
	if (mypair6.second != true)
	{
		cout << "key 5 插入失败" << endl;
	}
	else
	{
		cout << mypair6.first->first << "\t" << mypair6.first->second << endl;
	}

	//方法4
	map1[7] = "teacher07";
	map1[7] = "teacher77";

	//容器的遍历
	for (map<int, string>::iterator it = map1.begin(); it != map1.end(); it++)
	{
		cout << it->first << "\t" << it->second << endl;
	}
	cout << "遍历结束" << endl;

}

int main(void)
{
	cout << "-----------test1-----------" << endl;
	test1();
	cout << "-----------test2-----------" << endl;
	test2();
	cout << "-----------test3-----------" << endl;
	test3();
	return 0;
}