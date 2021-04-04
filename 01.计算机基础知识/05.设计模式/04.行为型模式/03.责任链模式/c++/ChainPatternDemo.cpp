#include <memory>
#include "AbstractLogger.h"
#include "ConsoleLogger.h"
#include "ErrorLogger.h"
#include "FileLogger.h"
using namespace std;

int main(void)
{
	shared_ptr<AbstractLogger> errorLogger(new ErrorLogger(ERROR));
	shared_ptr<AbstractLogger> fileLogger(new FileLogger(DEBUG));
	shared_ptr<AbstractLogger> consoleLogger(new ConsoleLogger(INFO));

	errorLogger->setNextLogger(fileLogger);
	fileLogger->setNextLogger(consoleLogger);

	errorLogger->logMessage(INFO, "This is an information.");

	errorLogger->logMessage(DEBUG, "This is an debug level information.");

	errorLogger->logMessage(ERROR, "This is an error information.");

	return 0;
}