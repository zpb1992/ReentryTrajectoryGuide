#include "Log.h"
#include"Console.h"

Log *Log::_instance = nullptr;

Log::Log()
{
	file();
}

bool Log::file()
{
	setDateFile();
	if (_ofs.is_open())
		_ofs.close();
	_ofs.open(_filename, std::ofstream::out | std::ofstream::app);
	printTime();
	return true;
}

bool Log::file(std::string filename)
{
	if (_ofs.is_open())
		_ofs.close();
	_ofs.open(_filename, std::ofstream::out | std::ofstream::app);
	printTime();
	return true;
}

Log::~Log()
{
	_ofs << std::endl;
	close();
}

Log * Log::instance()
{
	if (_instance == nullptr)
		_instance = new Log;
	return nullptr;
}

bool Log::close()
{
	if (_ofs)
		_ofs.close();
	return true;
}

bool Log::printTime()
{
	std::string t;
	time_t time_sec = time(nullptr);
	tm time;
	localtime_s(&time, &time_sec);
	t += "at ";
	t += std::to_string(time.tm_hour);
	t += ":";
	t += std::to_string(time.tm_min);
	t += ":";
	t += std::to_string(time.tm_sec);
	_ofs << t << std::endl;
	return false;
}

std::string & Log::setDateFile()
{
	time_t tempTime = time(nullptr);
	tm localTime;
	localtime_s(&localTime, &tempTime);
	_filename += std::to_string(localTime.tm_year + 1900);
	_filename += "_";
	_filename += std::to_string(localTime.tm_mon + 1);
	_filename += "_";
	_filename += std::to_string(localTime.tm_mday);
	_filename += ".log";

	return _filename;
}
