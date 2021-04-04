#include "ConsoleLogger.h"


ConsoleLogger::ConsoleLogger()
{
}

ConsoleLogger::ConsoleLogger(int level) :AbstractLogger(level)
{
}

void ConsoleLogger::write(string message) {
	cout << "Standard Console::Logger: " << message << endl;
}

ConsoleLogger::~ConsoleLogger()
{
}
