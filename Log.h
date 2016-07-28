#pragma once

#include<fstream>
#include<string>
#include<time.h>

class Log
{
private:
	Log();

public:
	bool file();
	bool file(std::string filename);
	~Log();

public:
	static Log * instance();
	template<typename T>
	bool write(T *data, unsigned num);
	bool close();

private:
	static Log *_instance;

	std::ofstream _ofs;
	std::string _filename;
private:
	bool printTime();
	std::string &setDateFile();
};

template<typename T>
inline bool Log::write(T * data, unsigned num)
{
	for (unsigned i = 0; i < num; ++i)
	{
		_ofs << data[i] << " ";
	}
	_ofs << std::endl;
	return true;
}
