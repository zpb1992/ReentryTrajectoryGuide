#pragma once

#include<string>

#define ToString(var) #var
	

class Configuration
{
public:
	Configuration(std::string filename);
	~Configuration();

public:
	virtual bool readConfig(std::string filename) = 0;
	virtual bool writeConfig(std::string filename) = 0;

protected:
	std::string _fileName;
	std::string getKeyByVar(std::string varName);
};

