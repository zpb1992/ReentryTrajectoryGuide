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

	// 粒子群规模
	unsigned _particleNum;
	unsigned _particleDimNum;
	unsigned *_particleDim;

	// 迭代公式 参数
	double _inertiaWeight;
	double _learningFactor1;
	double _learningFactor2;


	// 粒子群
	double ***_particlePositions;
	double ***_particleVelcities;
	double *_fitness;

	// 全局最优
	double **_bestPosition;
	double _bestFitness;

	// 局部最优
	double ***_localBestPosition;
	double *_localBestFitness;

	// 位置和速度的范围
	double **_positionScope;
	double **_velcityScope;

	// 迭代次数
	unsigned _iterations;

	// 状态量 指向Problem中的数据
	double **_state;
	unsigned _stateNum;
	unsigned *_stateDim;

	// 约束
	unsigned _pathStateNum;
	double *_pathStateMax;
	double *_pathStateWeights;
	double *_pathStateNormalization;
	unsigned _finalStateNum;
	double *_finalState;
	double *_finalStateNormalization;
	double *_finalStateWeights;
	unsigned _balanceNum;	//大小 变化率
	double *_balanceWeights;
	double *_balanceNormalization;
	unsigned _controlNum;	// 攻角 倾角
	double *_controlWeights;
	double *_controlNormalization;

public:
	bool readConfig(std::string filename) override;
	bool writeConfig(std::string filename) override;
public:
	virtual bool calculate();

	bool initialSwarm();			//  初始化位置 速度 		
	bool getFitnesses();			//	计算适应度
	bool updateLocalBests();		//  计算单个粒子的最优
	double updateGlobalBest();		//  计算粒子群的最优
	bool updateVelocities();		//  更新速度
	bool updatePositions();			//  更新位置
	bool iteration(unsigned iterations);				//  迭代
	bool clean();

private:
	bool initial(std::string filename);  // 动态数组
	bool initialParticle(unsigned index,bool initialBest=true);
	double getFitness(unsigned index);
	double updateLocalBest(unsigned index);
	bool updateVelocity(unsigned index);
	bool updatePosition(unsigned index);

	//bool getState();

// 发送控制量
public:
	//发送控制信号后 Reentry会计算State然后马上Send
	bool sendControl(unsigned index);
	bool sendControl(double **control);
	bool sendControl(double **control, unsigned *dim, unsigned num);

// 接受状态量
public:
	bool recvData(Array3<double> state) override;

	bool tieMediator(Mediator *mediator) override;
};

