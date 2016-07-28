#include "Configuration.h"
#include<cassert>
#include<string>


Configuration::Configuration(std::string filename)
{
	_fileName = filename;
}

Configuration::~Configuration()
{
}

std::string Configuration::getKeyByVar(std::string varName)
{
	assert(varName[0] == '_');

	varName.erase(0,1);
	if (varName[0]>='a'&&varName[0]<='z')
		varName[0] -= 'a' - 'A';
	else if (varName[0] >= 'A'&&varName[0] <= 'Z')
		;
	return varName;
}
