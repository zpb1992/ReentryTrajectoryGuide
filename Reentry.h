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

// �������ĳ�ʼ״̬Ҳ����Reentry�У���û������Aircraft��
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
	unsigned _stateDim; // Ӧ�ò�̫���ܲ�һ��������
	// 6��״̬ 3��·��Լ��״̬
	double **_state;

	double _c1; // �����ܶ�
	double _vc;


public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;

	bool initial(std::string filename);

	// ����״̬��
public:
	bool sendState();
	bool sendState(double **state);
	bool sendState(double **state, unsigned dim, unsigned num);

	// ���ܿ���(ǳ����)
public:
	bool recvData(Array3<double> control) override;

	// ����״̬��
public:
	bool calculateState();
	bool LGKT4();	// ���·������״̬
	bool DiffFunc(double *control, unsigned stateIndex, double *result);// ���һ��ʱ�̵�״̬
	bool DiffFunc(double *control, double *state, double *result);

	bool getPathState();
	double heatFlux(unsigned index);
	double dynamicPressure(unsigned index);
	double overload(unsigned index);

	double getCl(double attack, double velcity);
	double getCd(double attack, double velcity);
	double getLift(double airDensity, double velcity, double aircraftS, double Cl);
	double getDrag(double airDensity, double velcity, double   aricraftS, double Cd);

	// ͬʱ��ö��������
	bool getControls(double time, double * result, unsigned num = 2);
	// ���һ����һ�Ŀ�����
	double getOneControl(double time, double *control, unsigned dim);

	bool tieMediator(Mediator *mediator) override;

};

