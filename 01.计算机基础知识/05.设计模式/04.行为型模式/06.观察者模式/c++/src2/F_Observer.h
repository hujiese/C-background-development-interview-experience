#ifndef __F_Observer_H__
#define __F_Observer_H__

#include "Observer.h"
#include <string>
#include <iostream>
using namespace std;

class F_Observer :
	public Observer
{
public:
	F_Observer();
	virtual void update(string msg);
	virtual ~F_Observer();
};

#endif