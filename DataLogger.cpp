#include "DataLogger.h"
#include "Log.h"


DataLogger::DataLogger()
{
}


DataLogger::~DataLogger()
{
}

bool DataLogger::log(double * data, unsigned num)
{
	Log::instance()->write(data, num);
	return true;
}
