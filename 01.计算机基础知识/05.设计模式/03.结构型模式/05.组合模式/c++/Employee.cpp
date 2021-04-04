#include "Employee.h"


Employee::Employee()
{
}

Employee::Employee(string name, string dept, int sal) :name(name), dept(dept), salary(sal)
{
}

void Employee::add(shared_ptr<Employee> e)
{
	subordinates.push_back(e);
}

void Employee::remove(shared_ptr<Employee> e)
{
	subordinates.remove(e);
}

list<shared_ptr<Employee>> Employee::getSubordinates() const
{
	return subordinates;
}

string Employee::toString() const
{
	return ("Employee :[ Name : " + name + ", dept : " + dept + ", salary :" + to_string(salary) + " ]");
}

Employee::~Employee()
{
}
