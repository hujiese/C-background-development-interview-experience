#include<iostream>
#include<unordered_map>
#include<algorithm>
using namespace std;

//调试用
//void printList(int obj)
//{
//	cout << obj << "\t";
//}
//void printFun(unordered_map<int, list<int>> &obj)
//{
//	for (unordered_map<int, list<int>>::iterator it = obj.begin(); it != obj.end(); it++)
//	{
//		cout << it->first << "\t";
//		for_each(it->second.begin(), it->second.end(), printList);
//		cout << endl;
//	}
//}

class LFUCache
{
public:
	LFUCache(int cap = 0) :capicity(cap){}
	int get(const int key)
	{
		if (mData.count(key) <= 0)
			return -1;

		update(key);
		return mData[key].first;
	}

	void set(const int key, const int value)
	{
		if (mData.count(key) > 0)
		{
			mData[key] = make_pair(value, mData[key].second);
		}
		else
		{
			if (mData.size() == capicity)
			{
				for (unordered_map<int, list<int>>::iterator it = count_map.begin(); it != count_map.end(); ++it)
				{
					if (!it->second.empty())
					{
						int del_key = it->second.front();
						it->second.pop_front();
						mData.erase(del_key);
						mData.insert(make_pair(key, make_pair(value, 0)));
						break;
					}

				}
			}
			else
			{
				mData.insert(make_pair(key, make_pair(value, 0)));
			}
		}
		update(key);
	}

	// update the frequency
	void update(const int key)
	{
		list<int>::iterator it = find(count_map[mData[key].second].begin(), count_map[mData[key].second].end(), key);
		if (it != count_map[mData[key].second].end())
			count_map[mData[key].second].erase(it);
		mData[key].second++;
		count_map[mData[key].second].push_back(key);
	}
private:
	int capicity;
	unordered_map<int, pair<int, int>> mData;// key -- (value, frequency)
	unordered_map<int, list<int>> count_map;// frequency -- list of keys in the frequency
};




int main()
{
	LFUCache cache(3);
	cache.set(2, 2);
	cache.set(1, 1);
	cout << cache.get(2) << ",";
	cout << cache.get(1) << ",";
	cout << cache.get(2) << ",";
	cache.set(3, 3);
	cache.set(4, 4);
	cout << cache.get(3) << ",";
	cout << cache.get(2) << ",";
	cout << cache.get(1) << ",";
	cout << cache.get(4) << endl;


	//LFUCache cache(2);
	//cache.set(1, 1);
	//cache.set(2, 2);
	//cout << cache.get(1) << endl;       // 返回 1
	//cache.set(3, 3);    // 去除 key 2
	//cout << cache.get(2) << endl;       // 返回 -1 (未找到key 2)
	//cout << cache.get(3) << endl;       // 返回 3
	//cache.set(4, 4);    // 去除 key 1
	//cout << cache.get(1) << endl;       // 返回 -1 (未找到 key 1)
	//cout << cache.get(3) << endl;       // 返回 3
	//cout << cache.get(4) << endl;       // 返回 4

	system("pause");
	return 0;
}