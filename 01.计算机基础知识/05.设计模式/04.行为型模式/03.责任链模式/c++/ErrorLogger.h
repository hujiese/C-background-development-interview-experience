#pragma once
#include "AbstractLogger.h"
#include <iostream>
using namespace std;
class ErrorLogger :
	public AbstractLogger
{
public:
	ErrorLogger();
	ErrorLogger(int level);
	virtual ~ErrorLogger();

protected:
	void write(string message);
};

