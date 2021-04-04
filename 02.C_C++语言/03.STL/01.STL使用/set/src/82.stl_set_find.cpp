#include <iostream>
#include <set>
#include <string>
using namespace std;

int main(void)
{
	set<int> setInt;
	setInt.insert(3);
	setInt.insert(1);
	setInt.insert(7);
	setInt.insert(5);
	setInt.insert(9);

	cout << "---------find-----------" << endl;
	set<int>::iterator itA = setInt.find(5);
	int iA = *itA;		//iA == 5
	cout << iA << endl;

	cout << "---------count-----------" << endl;
	int iCount = setInt.count(5);	//iCount == 1
	cout << iCount << endl;

	cout << "---------lower/upper_bound-----------" << endl;
	set<int>::iterator itB = setInt.lower_bound(5);// >= 5
	set<int>::iterator itC = setInt.upper_bound(5);// > 5
	int iB = *itB;	//iB == 5
	int iC = *itC; //iC == 7
	cout << iB << "------" << iC << endl;

	cout << "---------equal_range-----------" << endl;
	pair< set<int>::iterator, set<int>::iterator > pairIt = setInt.equal_range(5);// [beg, end)
	set<int>::iterator itBeg = pairIt.first;
	set<int>::iterator itEnd = pairIt.second;
	cout << *itBeg << "----" << *itEnd << endl;

	return 0;
}