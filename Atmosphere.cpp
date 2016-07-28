#include "Atmosphere.h"
#include "json/json.h"
#include<fstream>	
#include<cmath>
#include"Console.h"

Atmosphere::Atmosphere(std::string filename):Configuration(filename)
{
	readConfig(filename);
}

Atmosphere::~Atmosphere()
{
}

bool Atmosphere::readConfig(std::string filename)
{
	Json::Reader reader;
	std::ifstream ifs(filename);
	Json::Value root;
	reader.parse(ifs, root);
	if (ifs)
	{
		_density = root[getKeyByVar(ToString(_density))].asDouble();
		_densityHeight = root[getKeyByVar(ToString(_densityHeight))].asDouble();
	}
	else
	{
		//Console::instance()->error("atmosphere read config");
	}


	return true;
}

bool Atmosphere::writeConfig(std::string filename)
{
	return false;
}

double Atmosphere::getAirDensity(double height)
{
	return _density*exp(-height/_densityHeight);
}

double Atmosphere::getAirDensity()
{
	return _density;
}
