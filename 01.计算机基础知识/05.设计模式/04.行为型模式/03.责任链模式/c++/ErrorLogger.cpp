#include "ErrorLogger.h"


ErrorLogger::ErrorLogger()
{
}

ErrorLogger::ErrorLogger(int level) :AbstractLogger(level)
{
}

void ErrorLogger::write(string message) {
	cout << "Error Console::Logger: " << message << endl;
}

ErrorLogger::~ErrorLogger()
{
}
