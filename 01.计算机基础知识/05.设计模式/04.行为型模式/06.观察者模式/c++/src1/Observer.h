#ifndef __OBSERVER_H
#define __OBSERVER_H
#include <memory>

using namespace std;

class Subject;
class Observer
{
public:
	Observer();
	virtual void update() = 0;
	virtual ~Observer();

protected:
	shared_ptr<Subject> subject;
};

#endif