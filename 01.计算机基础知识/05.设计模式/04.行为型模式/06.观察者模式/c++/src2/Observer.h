#ifndef __Observer_H__
#define __Observer_H__

#include <string>
using namespace std;

class Observer
{
public:
	Observer();
	virtual void update(string msg) = 0;
	virtual ~Observer();
};

#endif