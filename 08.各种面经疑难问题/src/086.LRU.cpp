#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;

template<class Key, class Value>
class Cache
{
public:
	Cache(int cap = 0) : capicity(cap){};
	int get(Key key, Value& value)
	{
		if (m_map.count(key) == 0)
		{
			return -1;
		}
		Iter it = m_map[key];
		value = it->second;
		m_list.splice(m_list.begin(), m_list, it);

		return 0;
	}

	void put(const Key& key, const Value& value)
	{
		if (m_map.count(key) >= 1)
		{
			Iter it = m_map[key];
			it->second = value;
			m_list.splice(m_list.begin(), m_list, it);
		}
		else
		{
			Node node(key, value);
			if (m_list.size() == capicity)
			{
				m_map.erase(m_list.back().first);
				m_list.pop_back();
			}
			m_list.push_front(node);
			m_map[key] = m_list.begin();
		}
	}

	void print()
	{
		for (pair<Key, Value> pi : m_list)
		{
			cout << pi.second << endl;
		}
	}
private:
	typedef pair<Key, Value> Node;
	typedef typename list<Node>::iterator Iter;
	list<Node> m_list;
	unordered_map<Key, Iter> m_map;
	int capicity;
};

int main(void)
{
	Cache<string, int> cache(5);
	cache.put("aaa", 1);
	cache.put("bbb", 2);
	cache.put("ccc", 3);
	cache.put("ddd", 4);
	cache.put("eee", 5);

	cache.print();
	cout << endl;

	int value = 0;
	cache.get("bbb", value);
	cache.print();
	cout << endl;

	cache.get("ddd", value);
	cache.print();
	cout << endl;

	cache.get("aaa", value);
	cache.print();
	cout << endl;

	cache.put("kkk", 6);
	cache.print();
	cout << endl;
	return 0;
}