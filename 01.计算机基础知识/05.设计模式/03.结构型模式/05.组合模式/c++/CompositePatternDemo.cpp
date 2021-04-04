#include "Employee.h"
using namespace std;

int main(void)
{
	shared_ptr<Employee> CEO(new Employee("John", "CEO", 30000));

	shared_ptr<Employee> headSales(new Employee("Robert", "Head Sales", 20000));

	shared_ptr<Employee> headMarketing(new Employee("Michel", "Head Marketing", 20000));

	shared_ptr<Employee> clerk1(new Employee("Laura", "Marketing", 10000));
	shared_ptr<Employee> clerk2(new Employee("Bob", "Marketing", 10000));

	shared_ptr<Employee> salesExecutive1(new Employee("Richard", "Sales", 10000));
	shared_ptr<Employee> salesExecutive2(new Employee("Rob", "Sales", 10000));

	CEO->add(headSales);
	CEO->add(headMarketing);

	headSales->add(salesExecutive1);
	headSales->add(salesExecutive2);

	headMarketing->add(clerk1);
	headMarketing->add(clerk2);

	//打印该组织的所有员工
	cout << CEO->toString() << endl;
	for (shared_ptr<Employee> headEmployee : CEO->getSubordinates()) {
		cout << headEmployee->toString() << endl;
		for (shared_ptr<Employee> employee : headEmployee->getSubordinates()) {
			cout << employee->toString() << endl;
		}
	}

	return 0;
}