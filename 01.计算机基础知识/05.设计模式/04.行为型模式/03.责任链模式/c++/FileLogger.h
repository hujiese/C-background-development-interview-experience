#pragma once
#include "AbstractLogger.h"
#include <iostream>
using namespace std;
class FileLogger :
	public AbstractLogger
{
public:
	FileLogger();
	FileLogger(int level);
	virtual ~FileLogger();

protected:
	void write(string message);
};

