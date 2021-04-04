#include <iostream>
#include <string>
#include <map>
using namespace std;

class Person
{
public:
	Person(string name, int age) :m_name(name), age(age)
	{
	}
	virtual void printT() const = 0;

protected:
	string	m_name;
	int		age;
};

class Teacher : public Person
{
public:
	Teacher(string name, int age, string id) : Person(name, age), m_id(id)
	{
	}
	void printT() const
	{
		cout << "name:" << m_name << " age:" << age << " m_id:" << m_id << endl;
 	}
protected:
	string	m_id;
};


//完成 老师结点 存储

class FlyWeightTeacherFactory 
{
public:
	explicit FlyWeightTeacherFactory()
	{
		map1.clear();
	}

	~FlyWeightTeacherFactory()
	{
		while ( !map1.empty())
		{
			Person *tmp = NULL;
			map<string, Person *>::iterator it = map1.begin();
			tmp = it->second;
			map1.erase(it); //把第一个结点 从容器中删除
			delete tmp;
		}
	}

	Person * GetTeacher(string id)
	{
		Person *tmp = NULL;
		map<string, Person *>::iterator it ;
		it = map1.find(id);
		if (it == map1.end()) //没有找到
		{
			string	tmpname;
			int		tmpage;
			cout << "\n请输入老师name:";
			cin >> tmpname;

			cout << "\n请输入老师age:";
			cin >> tmpage;

			tmp = new Teacher(tmpname, tmpage, id);
			map1.insert(pair<string, Person*>(id, tmp) );
		}
		else
		{
			tmp = it->second;
		}
		return tmp;
	}
private:
	map<string, Person *> map1;

};

int main(void)
{
	Person *p1 = NULL;
	Person *p2 = NULL;
	FlyWeightTeacherFactory *fwtf = new FlyWeightTeacherFactory;
	p1 = fwtf->GetTeacher("001");
	p1->printT();

	p2 = fwtf->GetTeacher("001");
	p2->printT();

	delete fwtf;

	return 0;
}