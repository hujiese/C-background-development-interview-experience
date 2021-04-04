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
	int m_iID;		//ѧ��
	string m_strName; 	//����
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
	setStu.insert(CStudent(3, "С��"));
	setStu.insert(CStudent(1, "С��"));
	setStu.insert(CStudent(5, "С��"));
	setStu.insert(CStudent(2, "С��"));
	//��ʱ����setStu�������ĸ�ѧ�����󣬷ֱ��ǰ�����˳��ġ�С�����С��������С�š�����С���� 
	for (set<CStudent, StuFunctor>::iterator it = setStu.begin(); it != setStu.end(); it++)
	{
		cout << it->m_iID << "-" << it->m_strName <<  endl;
	}
	
}
