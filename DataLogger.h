#pragma once
class DataLogger
{
public:
	DataLogger();
	~DataLogger();

public:
	bool log(double *data, unsigned num);
};

