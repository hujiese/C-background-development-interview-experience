#pragma once
#include "AbstractLogger.h"
#include <iostream>
using namespace std;

class ConsoleLogger :
	public AbstractLogger
{
public:
	ConsoleLogger();
	ConsoleLogger(int level);
	virtual ~ConsoleLogger();

protected:
	virtual void write(string message);
};

