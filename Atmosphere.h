#pragma once

#include "Configuration.h"

class Atmosphere:
	public Configuration
{
public:
	Atmosphere(std::string filename="conf/Atmosphere.json");
	~Atmosphere();

private:
	double _density;		// 海平面大气密度
	double _densityHeight;	// 密度高度
public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;
	double getAirDensity(double height);
	double getAirDensity();  // 海平面大气密度
};

