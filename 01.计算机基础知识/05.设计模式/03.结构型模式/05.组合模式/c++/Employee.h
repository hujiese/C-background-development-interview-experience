#pragma once
#include <iostream>
#include <string>
#include <list>
#include <memory>
using namespace std;

class Employee
{
public:
	explicit Employee();
	Employee(string name, string dept, int sal);
	void add(shared_ptr<Employee> e);
	void remove(shared_ptr<Employee> e);
	list<shared_ptr<Employee>> getSubordinates() const;
	string toString() const;
	virtual ~Employee();

private:
	string name;
	string dept;
	int salary;
	list<shared_ptr<Employee>> subordinates;
};

