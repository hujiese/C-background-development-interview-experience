#ifndef __S_Observer_H__
#define __S_Observer_H__

#include "Observer.h"
#include <string>
#include <iostream>
using namespace std;
class S_Observer :
	public Observer
{
public:
	S_Observer();
	virtual void update(string msg);
	virtual ~S_Observer();
};

#endif