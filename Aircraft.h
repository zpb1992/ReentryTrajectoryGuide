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
	double getM();// ����
	double getS();// �����ο����
	double getR();// ���ʰ뾶

};

