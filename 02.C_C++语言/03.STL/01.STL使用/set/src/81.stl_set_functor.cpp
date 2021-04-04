#include <iostream>
#include <set>
#include <string>
using namespace std;

class CStudent
{
public:
	CStudent(int iID, string strName)
	{
		m_iID = iID;
		m_strName = strName;
	}
	int m_iID;		//学号
	string m_strName; 	//姓名
};

struct StuFunctor
{
	bool operator()  (const CStudent &stu1, const CStudent &stu2)
	{
		return (stu1.m_iID<stu2.m_iID);
	}
};

void main()
{
	set<CStudent, StuFunctor> setStu;
	setStu.insert(CStudent(3, "小张"));
	setStu.insert(CStudent(1, "小李"));
	setStu.insert(CStudent(5, "小王"));
	setStu.insert(CStudent(2, "小刘"));
	//此时容器setStu包含了四个学生对象，分别是按姓名顺序的“小李”，“小刘”，“小张”，“小王” 
	for (set<CStudent, StuFunctor>::iterator it = setStu.begin(); it != setStu.end(); it++)
	{
		cout << it->m_iID << "-" << it->m_strName <<  endl;
	}
	
}
