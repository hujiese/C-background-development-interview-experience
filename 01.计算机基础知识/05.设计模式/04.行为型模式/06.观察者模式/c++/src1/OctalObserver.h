#ifndef __OCTALOBSERVER_H__ 
#define __OCTALOBSERVER_H__
#pragma once
#include "Observer.h"
#include "Subject.h"
#include <iostream>
	using namespace std;

class OctalObserver :
	public Observer
{
public:
	OctalObserver(shared_ptr<Subject> subject);
	virtual void update();
	virtual ~OctalObserver();
};


#endif