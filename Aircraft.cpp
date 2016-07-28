#include "Aircraft.h"
#include "json\json.h"
#include <fstream>

Aircraft::Aircraft(std::string filename):Configuration(filename)
{
	readConfig(filename);
}

Aircraft::~Aircraft()
{
}

bool Aircraft::readConfig(std::string filename)
{
	Json::Reader reader;
	std::ifstream ifs(filename);
	Json::Value root;
	reader.parse(ifs, root);

	_mass = root[getKeyByVar(ToString(_mass))].asDouble();
	_s = root[getKeyByVar(ToString(_s))].asDouble();	
	_r = root[getKeyByVar(ToString(_r))].asDouble();

	return true;
}

bool Aircraft::writeConfig(std::string filename)
{
	return false;
}

double Aircraft::getM()
{
	return _mass;
}

double Aircraft::getS()
{
	return _s;
}

double Aircraft::getR()
{
	return _r;
}
