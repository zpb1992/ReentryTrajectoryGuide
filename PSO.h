#pragma once
#include "Algorithm.h"
#include "Configuration.h"
#include "Problem.h"
#include "Mediator.h"
#include "Array3.h"
#include "Component.h"
#include "DataLogger.h"

class PSO :
	public Algorithm, public Configuration,public Component,public DataLogger
{
public:
	PSO(std::string filename="conf/PSO.json");
	~PSO() override;

private:
	Mediator *_mediator;

	// ����Ⱥ��ģ
	unsigned _particleNum;
	unsigned _particleDimNum;
	unsigned *_particleDim;

	// ������ʽ ����
	double _inertiaWeight;
	double _learningFactor1;
	double _learningFactor2;


	// ����Ⱥ
	double ***_particlePositions;
	double ***_particleVelcities;
	double *_fitness;

	// ȫ������
	double **_bestPosition;
	double _bestFitness;

	// �ֲ�����
	double ***_localBestPosition;
	double *_localBestFitness;

	// λ�ú��ٶȵķ�Χ
	double **_positionScope;
	double **_velcityScope;

	// ��������
	unsigned _iterations;

	// ״̬�� ָ��Problem�е�����
	double **_state;
	unsigned _stateNum;
	unsigned *_stateDim;

	// Լ��
	unsigned _pathStateNum;
	double *_pathStateMax;
	double *_pathStateWeights;
	double *_pathStateNormalization;
	unsigned _finalStateNum;
	double *_finalState;
	double *_finalStateNormalization;
	double *_finalStateWeights;
	unsigned _balanceNum;	//��С �仯��
	double *_balanceWeights;
	double *_balanceNormalization;
	unsigned _controlNum;	// ���� ���
	double *_controlWeights;
	double *_controlNormalization;

public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;
public:
	virtual bool calculate();

	bool initialSwarm();			//  ��ʼ��λ�� �ٶ� 		
	bool getFitnesses();			//	������Ӧ��
	bool updateLocalBests();		//  ���㵥�����ӵ�����
	double updateGlobalBest();		//  ��������Ⱥ������
	bool updateVelocities();		//  �����ٶ�
	bool updatePositions();			//  ����λ��
	bool iteration(unsigned iterations);				//  ����
	bool clean();

private:
	bool initial(std::string filename);  // ��̬����
	bool initialParticle(unsigned index,bool initialBest=true);
	double getFitness(unsigned index);
	double updateLocalBest(unsigned index);
	bool updateVelocity(unsigned index);
	bool updatePosition(unsigned index);

	//bool getState();

// ���Ϳ�����
public:
	//���Ϳ����źź� Reentry�����StateȻ������Send
	bool sendControl(unsigned index);
	bool sendControl(double **control);
	bool sendControl(double **control, unsigned *dim, unsigned num);

// ����״̬��
public:
	bool recvData(Array3<double> state) override;

	bool tieMediator(Mediator *mediator) override;
};

