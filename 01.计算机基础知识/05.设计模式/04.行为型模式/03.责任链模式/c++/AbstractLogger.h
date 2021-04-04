#pragma once
#include <memory>
#include <string>
#define  INFO  1
#define  DEBUG 2
#define  ERROR 3
using namespace std;

class AbstractLogger
{
public:
	AbstractLogger();
	AbstractLogger(int level);
	void setNextLogger(weak_ptr<AbstractLogger> nextLogger);
	void logMessage(int level, string message);
	virtual ~AbstractLogger();

protected:
	int level;
	weak_ptr<AbstractLogger> nextLogger;
	virtual void write(string message);
};

