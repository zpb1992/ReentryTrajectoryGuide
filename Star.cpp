#include "Star.h"
#include "json\json.h"
#include<fstream>
#include"Math.h"

Star::Star(std::string filename) :Configuration(filename)
{
	readConfig(_fileName);
}

Star::~Star()
{
}

bool Star::readConfig(std::string filename)
{
	Json::Reader reader;
	std::ifstream ifs(filename);
	Json::Value root;
	reader.parse(ifs, root);

	_rotationW = 2.0*Math::PI / (24 * 3600);
	_radius = root[getKeyByVar(ToString(_radius))].asDouble();
	_gravityAcc = root[getKeyByVar(ToString(_gravityAcc))].asDouble();

	return true;
}

bool Star::writeConfig(std::string filename)
{
	return false;
}

double Star::getGravityAcc(double radius)
{
	return _radius*_radius*_gravityAcc / (radius*radius);
}

double Star::getRotationW()
{
	return _rotationW;
}

double Star::getRadius()
{
	return _radius;
}
