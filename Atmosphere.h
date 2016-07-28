#pragma once

#include "Configuration.h"

class Atmosphere:
	public Configuration
{
public:
	Atmosphere(std::string filename="conf/Atmosphere.json");
	~Atmosphere();

private:
	double _density;		// ��ƽ������ܶ�
	double _densityHeight;	// �ܶȸ߶�
public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;
	double getAirDensity(double height);
	double getAirDensity();  // ��ƽ������ܶ�
};

