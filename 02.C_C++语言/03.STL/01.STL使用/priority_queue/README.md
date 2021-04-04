## STL -- priority_queue（了解）

操作函数示例：
```c
priority_queue<int, deque<int>> 	pq;
priority_queue<int, vector<int>> 	pq;
pq.empty()
pq.size()
pq.top()
pq.pop()
pq.push(item)
```
使用案例：
```c
#include <iostream>
#include <queue>
#include <functional>
using namespace std;

void main()
{
	priority_queue<int> p1; //默认是 最大值优先级队列 
	//priority_queue<int, vector<int>, less<int> > p1; //相当于这样写
	priority_queue<int, vector<int>, greater<int>> p2; //最小值优先级队列

	p1.push(33);
	p1.push(11);
	p1.push(55);
	p1.push(22);
	cout << "队列大小" << p1.size() << endl;
	cout << "队头" << p1.top() << endl;

	while (p1.size() > 0)
	{
		cout << p1.top() << " ";
		p1.pop();
	}
	cout << endl;

	cout << "测试 最小值优先级队列" << endl;
	p2.push(33);
	p2.push(11);
	p2.push(55);
	p2.push(22);
	while (p2.size() > 0)
	{
		cout << p2.top() << " ";
		p2.pop();

	}
}
```
测试结果如下：
```
队列大小4
队头55
55 33 22 11
测试 最小值优先级队列
11 22 33 55
```