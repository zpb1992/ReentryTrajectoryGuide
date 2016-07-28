#pragma once

#include "Configuration.h"

class Aircraft:
	public Configuration
{
public:
	Aircraft(std::string filename="conf/CAV_L.json");
	~Aircraft();
private:
	double _mass;
	double _s;
	double _r;
public:
	bool readConfig(std::string filename);
	bool writeConfig(std::string filename);
	double getM();// 质量
	double getS();// 气动参考面积
	double getR();// 曲率半径

};

