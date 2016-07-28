#pragma once

#include "Problem.h"
#include "Configuration.h"
#include "Aircraft.h"
#include "Mediator.h"
#include "Atmosphere.h"
#include "Star.h"
#include "Array3.h"
#include "Component.h"
#include "DataLogger.h"

// 飞行器的初始状态也算在Reentry中，而没有算在Aircraft中
class Reentry :
	public Problem, public Configuration, public Component,public DataLogger
{
public:
	Reentry(std::string filename = "conf/Reentry.json");
	~Reentry();

private:
	Mediator *_mediator;

	Aircraft _aircraft;
	Atmosphere _atmosphere;
	Star _star;

	unsigned _controlNum; //3
	unsigned *_controlDim;
	double **_control;

	unsigned _stateNum;
	unsigned _pathContraintsNum;
	unsigned _stateDim; // 应该不太可能不一样。。。
	// 6个状态 3个路径约束状态
	double **_state;

	double _c1; // 热流密度
	double _vc;


public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;

	bool initial(std::string filename);

	// 发送状态量
public:
	bool sendState();
	bool sendState(double **state);
	bool sendState(double **state, unsigned dim, unsigned num);

	// 接受控制(浅拷贝)
public:
	bool recvData(Array3<double> control) override;

	// 计算状态量
public:
	bool calculateState();
	bool LGKT4();	// 求解路径所有状态
	bool DiffFunc(double *control, unsigned stateIndex, double *result);// 求解一个时刻的状态
	bool DiffFunc(double *control, double *state, double *result);

	bool getPathState();
	double heatFlux(unsigned index);
	double dynamicPressure(unsigned index);
	double overload(unsigned index);

	double getCl(double attack, double velcity);
	double getCd(double attack, double velcity);
	double getLift(double airDensity, double velcity, double aircraftS, double Cl);
	double getDrag(double airDensity, double velcity, double   aricraftS, double Cd);

	// 同时获得多个控制量
	bool getControls(double time, double * result, unsigned num = 2);
	// 获得一个单一的控制量
	double getOneControl(double time, double *control, unsigned dim);

	bool tieMediator(Mediator *mediator) override;

};

