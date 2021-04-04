#ifndef __HEXOBSERVER_H__
#define __HEXOBSERVER_H__

#pragma once
#include "Observer.h"
#include "Subject.h"
#include <iostream>
using namespace std;

class HexaObserver :
	public Observer
{
public:
	HexaObserver(shared_ptr<Subject> subject);
	virtual void update();
	virtual ~HexaObserver();
};


#endif
