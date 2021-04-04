#include <iostream>
#include <map>
#include <string>
using namespace std;

class Person
{
public:
	string    name;
	int       age;
	string    tel;
	double    salary;
};

void test1(void)
{
	Person p1, p2, p3, p4, p5;
	p1.name = "person1";
	p1.age = 1;

	p2.name = "person2";
	p2.age = 2;

	p3.name = "person3";
	p3.age = 3;

	p4.name = "person4";
	p4.age = 4;

	p5.name = "person5";
	p5.age = 5;

	multimap<string, Person> mp;
	mp.insert(make_pair("sale", p1));
	mp.insert(make_pair("sale", p2));
	mp.insert(make_pair("development", p3));
	mp.insert(make_pair("development", p4));
	mp.insert(make_pair("Financial", p5));

	for (multimap<string, Person>::iterator it = mp.begin(); it != mp.end(); it++)
	{
		cout << it->first << "\t" << it->second.name << endl; 
	}
}

void test2(void)
{
	Person p1, p2, p3, p4, p5;
	p1.name = "person1";
	p1.age = 1;

	p2.name = "person2";
	p2.age = 2;

	p3.name = "person3";
	p3.age = 3;

	p4.name = "person4";
	p4.age = 4;

	p5.name = "person5";
	p5.age = 5;

	multimap<string, Person> mp;
	mp.insert(make_pair("sale", p1));
	mp.insert(make_pair("sale", p2));
	mp.insert(make_pair("development", p3));
	mp.insert(make_pair("development", p4));
	mp.insert(make_pair("Financial", p5));

	multimap<string, Person>::iterator it = mp.find("development");
	int len = mp.count("development");
	int count = 0;
	while ((it != mp.end()) && (count < len))
	{
		cout << it->first << "\t" << it->second.age << endl;
		it++;
		count++;
	}
}


void test3(void)
{
	Person p1, p2, p3, p4, p5;
	p1.name = "person1";
	p1.age = 1;

	p2.name = "person2";
	p2.age = 2;

	p3.name = "person3";
	p3.age = 3;

	p4.name = "person4";
	p4.age = 4;

	p5.name = "person5";
	p5.age = 5;

	multimap<string, Person> mp;
	mp.insert(make_pair("sale", p1));
	mp.insert(make_pair("sale", p2));
	mp.insert(make_pair("development", p3));
	mp.insert(make_pair("development", p4));
	mp.insert(make_pair("Financial", p5));

	for (multimap<string, Person>::iterator it = mp.begin(); it != mp.end(); it++)
	{
		if (it->second.name == "person4")
		{
			it->second.name = "personFourth";
		}
	}

	for (multimap<string, Person>::iterator it = mp.begin(); it != mp.end(); it++)
	{
		cout << it->first << "\t" << it->second.name << endl;
	}

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