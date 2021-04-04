#ifndef __BINARYOBSERVER_H__
#define __BINARYOBSERVER_H__

#pragma once
#include "Observer.h"
#include "Subject.h"
#include <iostream>
using namespace std;

class BinaryObserver :
	public Observer
{
public:
	BinaryObserver(shared_ptr<Subject> subject);
	virtual void update();
	virtual ~BinaryObserver();
};

#endif