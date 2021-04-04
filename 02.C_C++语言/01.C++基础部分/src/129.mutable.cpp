#include <iostream>
#define USING_MUTABLE
using namespace std;

class ClxTest
{
public:
	ClxTest();
	~ClxTest();

	void Output() const;
	int GetOutputTimes() const;

private:
#ifdef USING_MUTABLE
	mutable int m_iTimes;
#else
	int m_iTimes;
#endif
};

ClxTest::ClxTest()
{
	m_iTimes = 0;
}

ClxTest::~ClxTest()
{}

void ClxTest::Output() const
{
	cout << "Output for test!" << endl;
	m_iTimes++; // 	如果不加mutalbe则报错:  表达式必须是可修改的左值
}

int ClxTest::GetOutputTimes() const
{
	return m_iTimes;
}

void OutputTest(const ClxTest& lx)
{
	cout << lx.GetOutputTimes() << endl;
	lx.Output();
	cout << lx.GetOutputTimes() << endl;
}