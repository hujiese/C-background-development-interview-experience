#include "AbstractLogger.h"


AbstractLogger::AbstractLogger()
{
}

AbstractLogger::AbstractLogger(int level) :level(level)
{
}

void AbstractLogger::setNextLogger(weak_ptr<AbstractLogger> nextLogger)
{
	this->nextLogger = nextLogger;
}

void AbstractLogger::logMessage(int level, string message)
{
	if (this->level <= level){
		write(message);
	}
	if (!nextLogger.expired()){
		shared_ptr<AbstractLogger> temp = nextLogger.lock();
		temp->logMessage(level, message);
	}
}

void AbstractLogger::write(string message)
{
}

AbstractLogger::~AbstractLogger()
{
}
