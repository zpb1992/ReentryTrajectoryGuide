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
	//double _mass; //涉及大数的保存 计算 
	double _radius;
	double _gravityAcc;
public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;

public:
	double getGravityAcc(double radius);  // radius是物体到star中心的距离
	double getRotationW();
	double getRadius();
};

