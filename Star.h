#pragma once

#include "Configuration.h"

class Star:
	public Configuration
{
public:
	Star(std::string filename="conf/Earth.json");
	~Star();
private:
	double _rotationW;
	//double _mass; //�漰�����ı��� ���� 
	double _radius;
	double _gravityAcc;
public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;

public:
	double getGravityAcc(double radius);  // radius�����嵽star���ĵľ���
	double getRotationW();
	double getRadius();
};

