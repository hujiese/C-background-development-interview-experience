#include "FileLogger.h"


FileLogger::FileLogger()
{
}

FileLogger::FileLogger(int level) :AbstractLogger(level)
{
}

void FileLogger::write(string message) {
	cout << "File::Logger: " << message << endl;
}

FileLogger::~FileLogger()
{
}
