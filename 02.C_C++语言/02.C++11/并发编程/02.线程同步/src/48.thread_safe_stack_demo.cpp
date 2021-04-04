#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>

using namespace std;

mutex myMutex;

class stack
{
public:
	stack() {};
	~stack() {};
	void pop();
	int top() { return data.back(); }
	void push(int);
	void print();
	int getSize() { return data.size(); }
private:
	vector<int> data;
};

void stack::pop()
{
	lock_guard<mutex> guard(myMutex);
	data.erase(data.end() - 1);
}

void stack::push(int n)
{
	lock_guard<mutex> guard(myMutex);
	data.push_back(n);
}

void stack::print()
{
	cout << "initial stack : ";
	for (int item : data)
		cout << item << " ";
	cout << endl;
}

void process(int val, string s)
{
	lock_guard<mutex> guard(myMutex);
	cout << s << " : " << val << endl;
}

void thread_function(stack& st, string s)
{
	int val = st.top();
	st.pop();
	process(val, s);
}

int main()
{
	stack st;
	for (int i = 0; i < 10; i++)  st.push(i);

	st.print();

	while (true) {
		if (st.getSize() > 0) {
			thread t1(&thread_function, ref(st), string("thread1"));
			t1.join();
		}
		else
			break;
		if (st.getSize() > 0) {
			thread t2(&thread_function, ref(st), string("thread2"));
			t2.join();
		}
		else
			break;
	}

	return 0;
}