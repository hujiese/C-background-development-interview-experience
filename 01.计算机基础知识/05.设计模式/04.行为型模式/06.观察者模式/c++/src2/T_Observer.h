#ifndef __T_Observer_H__
#define __T_Observer_H__

#include "Observer.h"
#include <string>
#include <iostream>
using namespace std;
class T_Observer :
	public Observer
{
public:
	T_Observer();
	virtual void update(string msg);
	virtual ~T_Observer();
};

#endif