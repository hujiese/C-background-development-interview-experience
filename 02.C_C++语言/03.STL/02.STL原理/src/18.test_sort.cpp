#define _CRT_SECURE_NO_WARNINGS
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <functional>
using namespace std;

namespace jj36
{
	bool myfunc(int i, int j) { return (i<j); }

	struct myclass {
		bool operator() (int i, int j) { return (i<j); }
	} myobj;

	bool test_sort()
	{
		cout << "\ntest_sort().......... \n";

		int myints[] = { 32, 71, 12, 45, 26, 80, 53, 33 };
		vector<int> myvec(myints, myints + 8);          // 32 71 12 45 26 80 53 33

		// using default comparison (operator <):
		sort(myvec.begin(), myvec.begin() + 4);         //(12 32 45 71)26 80 53 33

		// using function as comp
		sort(myvec.begin() + 4, myvec.end(), myfunc); 	// 12 32 45 71(26 33 53 80)

		// using object as comp
		sort(myvec.begin(), myvec.end(), myobj);      //(12 26 32 33 45 53 71 80)

		// print out content:
		cout << "\nmyvec contains:";
		for (auto elem : myvec)		//C++11 range-based for statement
			cout << ' ' << elem; 	//output: 12 26 32 33 45 53 71 80

		// using reverse iterators and default comparison (operator <):
		sort(myvec.rbegin(), myvec.rend());

		// print out content:
		cout << "\nmyvec contains:";
		for (auto elem : myvec)		//C++11 range-based for statement
			cout << ' ' << elem; 	//output: 80 71 53 45 33 32 26 12    

		// using explicitly default comparison (operator <):
		sort(myvec.begin(), myvec.end(), less<int>());

		// print out content:
		cout << "\nmyvec contains:";
		for (auto elem : myvec)		//C++11 range-based for statement
			cout << ' ' << elem; 	//output: 12 26 32 33 45 53 71 80   

		// using another comparision criteria (operator >):
		sort(myvec.begin(), myvec.end(), greater<int>());

		// print out content:
		cout << "\nmyvec contains:";
		for (auto elem : myvec)		//C++11 range-based for statement
			cout << ' ' << elem; 	//output: 80 71 53 45 33 32 26 12 

		return true;
	}
}

int main(void)
{
	jj36::test_sort();
	return 0;
}