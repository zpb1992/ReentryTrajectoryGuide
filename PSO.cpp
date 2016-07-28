#include "PSO.h"
#include "json/json.h"
#include <fstream>
#include "Math.h"
#include<cassert>
#include"Console.h"

PSO::PSO(std::string filename) :Configuration(filename)
{
}

PSO::~PSO()
{
	clean();
}

bool PSO::readConfig(std::string filename)
{
	Json::Reader reader;
	std::ifstream ifs(filename);
	Json::Value root;
	reader.parse(ifs, root);

	for (unsigned i = 0; i < _particleDimNum; ++i)
	{
		if (i != 2)
		{
			_positionScope[i][0] = Math::getRadian(root[getKeyByVar(ToString(_positionScope))][i]["min"].asDouble());
			_positionScope[i][1] = Math::getRadian(root[getKeyByVar(ToString(_positionScope))][i]["max"].asDouble());
			_velcityScope[i][0] = Math::getRadian(root[getKeyByVar(ToString(_velcityScope))][i]["min"].asDouble());
			_velcityScope[i][1] = Math::getRadian(root[getKeyByVar(ToString(_velcityScope))][i]["max"].asDouble());
		}
		else // final time
		{
			_positionScope[i][0] = root[getKeyByVar(ToString(_positionScope))][i]["min"].asDouble();
			_positionScope[i][1] = root[getKeyByVar(ToString(_positionScope))][i]["max"].asDouble();
			_velcityScope[i][0] = root[getKeyByVar(ToString(_velcityScope))][i]["min"].asDouble();
			_velcityScope[i][1] = root[getKeyByVar(ToString(_velcityScope))][i]["max"].asDouble();
		}

	}
	_iterations = root[getKeyByVar(ToString(_iterations))].asUInt();
	_inertiaWeight = root[getKeyByVar(ToString(_inertiaWeight))].asDouble();
	_learningFactor1 = root[getKeyByVar(ToString(_learningFactor1))].asDouble();
	_learningFactor2 = root[getKeyByVar(ToString(_learningFactor2))].asDouble();


	for (unsigned i = 0; i < _pathStateNum; ++i)
	{
		_pathStateMax[i] = root[getKeyByVar(ToString(_pathStateMax))][i].asDouble();
		_pathStateWeights[i] = root[getKeyByVar(ToString(_pathStateWeights))][i].asDouble();
		_pathStateNormalization[i] = root[getKeyByVar(ToString(_pathStateNormalization))][i].asDouble();
	}
	for (unsigned i = 0; i < _finalStateNum; ++i)
	{
		if (1 == i || 2 == i || 4 == i || 5 == i)
		{
			_finalState[i] = Math::getRadian(root[getKeyByVar(ToString(_finalState))][i].asDouble());
		}
		else
		{
			_finalState[i] = root[getKeyByVar(ToString(_finalState))][i].asDouble();
		}
		_finalStateWeights[i] = root[getKeyByVar(ToString(_finalStateWeights))][i].asDouble();
		_finalStateNormalization[i] = root[getKeyByVar(ToString(_finalStateNormalization))][i].asDouble();
	}
	for (unsigned i = 0; i < _balanceNum; ++i)
	{
		_balanceWeights[i] = Math::getRadian(root[getKeyByVar(ToString(_balanceWeights))][i].asDouble());
		_balanceNormalization[i] = root[getKeyByVar(ToString(_balanceNormalization))][i].asDouble();
	}
	for (unsigned i = 0; i < _controlNum; ++i)
	{
		_controlWeights[i] = Math::getRadian(root[getKeyByVar(ToString(_controlWeights))][i].asDouble());
		_controlNormalization[i] = root[getKeyByVar(ToString(_controlNormalization))][i].asDouble();
	}

	return true;
}

bool PSO::writeConfig(std::string filename)
{
	Json::Value root;

	root[getKeyByVar(ToString(_particleDimNum))] = _particleDimNum;
	root[getKeyByVar(ToString(_particleNum))] = _particleNum;
	for (unsigned i = 0; i < _particleDimNum; ++i)
	{
		root[getKeyByVar(ToString(_particleDim))][i] = _particleDim[i];
		root[getKeyByVar(ToString(_positionScope))][i]["min"] = _positionScope[i][0];
		root[getKeyByVar(ToString(_positionScope))][i]["max"] = _positionScope[i][1];
		root[getKeyByVar(ToString(_velcityScope))][i]["min"] = _velcityScope[i][0];
		root[getKeyByVar(ToString(_velcityScope))][i]["max"] = _velcityScope[i][1];
	}

	root[getKeyByVar(ToString(_iterations))] = _iterations;
	root[getKeyByVar(ToString(_inertiaWeight))] = _inertiaWeight;
	root[getKeyByVar(ToString(_learningFactor1))] = _learningFactor1;
	root[getKeyByVar(ToString(_learningFactor2))] = _learningFactor2;


	Json::StyledStreamWriter writer;
	std::ofstream ofs(filename);
	writer.write(ofs, root);

	return true;
}

bool PSO::calculate()
{
	initial(_fileName);
	readConfig(_fileName);
	initialSwarm();
	unsigned iterations = 0;

	while (!iteration(iterations))
	{
		++iterations;
	}

	return true;
}

bool PSO::initialSwarm()
{
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		initialParticle(i);
	}
	getFitnesses();
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		_localBestFitness[i] = _fitness[i];
	}
	// 先初始化最优为第一个粒子吧
	for (unsigned j = 0; j < _particleDimNum; ++j)
	{
		for (unsigned m = 0; m < _particleDim[j]; ++m)
		{
			_bestPosition[j][m] = _particlePositions[0][j][m];
		}
	}
	_bestFitness = _fitness[0];
	return true;
}

bool PSO::getFitnesses()
{
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		_fitness[i] = getFitness(i);
	}
	return true;
}

bool PSO::updateLocalBests()
{
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		updateLocalBest(i);
	}
	return true;
}

double PSO::updateGlobalBest()
{
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		if (_localBestFitness[i] > _bestFitness)
		{
			_bestFitness = _localBestFitness[i];
			for (unsigned j = 0; j < _particleDimNum; ++j)
			{
				for (unsigned m = 0; m < _particleDim[j]; ++m)
				{
					_bestPosition[j][m] = _localBestPosition[i][j][m];
				}
			}
		}
	}
	return _bestFitness;
}

bool PSO::updateVelocities()
{
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		updateVelocity(i);
	}
	return true;
}

bool PSO::updatePositions()
{
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		updatePosition(i);
	}
	return true;
}

bool PSO::iteration(unsigned iterations)
{
	updateVelocities();
	updatePositions();
	getFitnesses();
	updateLocalBests();
	updateGlobalBest();

	if (iterations >= _iterations)
		return true;
	else
		return false;
}

bool PSO::clean()
{
	delete _particleDim;
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		for (unsigned j = 0; j < _particleDimNum; ++j)
		{
			delete _particlePositions[i][j];
			delete _particleVelcities[i][j];
			delete _localBestPosition[i][j];
		}
		delete _particlePositions[i];
		delete _particleVelcities[i];
		delete _localBestPosition[i];
	}
	for (unsigned j = 0; j < _particleDimNum; ++j)
	{
		delete _bestPosition[j];
		delete _positionScope[j];
		delete _velcityScope[j];
	}
	delete _particlePositions;
	delete _particleVelcities;
	delete _bestPosition;
	delete _localBestPosition;
	delete _localBestFitness;
	delete _positionScope;
	delete _velcityScope;

	delete _pathStateMax;
	delete _pathStateWeights;
	delete _finalState;
	delete _finalStateWeights;
	delete _balanceWeights;
	delete _controlWeights;

	return true;
}


bool PSO::initial(std::string filename)
{
	Json::Reader reader;
	std::ifstream ifs(filename);
	Json::Value root;
	reader.parse(ifs, root);

	_particleDimNum = root[getKeyByVar(ToString(_particleDim))].size();
	_particleDim = new unsigned[_particleDimNum];
	for (unsigned i = 0; i < _particleDimNum; ++i)
	{
		_particleDim[i] = root[getKeyByVar(ToString(_particleDim))][i].asUInt();
	}
	_particleNum = root[getKeyByVar(ToString(_particleNum))].asUInt();

	_particlePositions = new double **[_particleNum];
	_particleVelcities = new double **[_particleNum];
	_fitness = new double[_particleNum];
	_localBestPosition = new double **[_particleNum];
	_localBestFitness = new double[_particleNum];
	_bestPosition = new double *[_particleDimNum];
	_positionScope = new double *[_particleDimNum];
	_velcityScope = new double*[_particleDimNum];
	for (unsigned i = 0; i < _particleNum; ++i)
	{
		_particlePositions[i] = new double *[_particleDimNum];
		_particleVelcities[i] = new double *[_particleDimNum];
		_localBestPosition[i] = new double *[_particleDimNum];
		for (unsigned j = 0; j < _particleDimNum; ++j)
		{
			_particlePositions[i][j] = new double[_particleDim[j]];
			_particleVelcities[i][j] = new double[_particleDim[j]];
			_localBestPosition[i][j] = new double[_particleDim[j]];
		}
	}

	for (unsigned i = 0; i < _particleDimNum; ++i)
	{
		_bestPosition[i] = new double[_particleDim[i]];
		_positionScope[i] = new double[2];
		_velcityScope[i] = new double[2];
	}

	_pathStateNum = root[getKeyByVar(ToString(_pathStateWeights))].size();
	_finalStateNum = root[getKeyByVar(ToString(_finalStateWeights))].size();
	_balanceNum = root[getKeyByVar(ToString(_balanceWeights))].size();
	_controlNum = root[getKeyByVar(ToString(_controlWeights))].size();

	_pathStateMax = new double[_pathStateNum];
	_pathStateWeights = new double[_pathStateNum];
	_pathStateNormalization = new double[_pathStateNum];
	_finalState = new double[_finalStateNum];
	_finalStateWeights = new double[_finalStateNum];
	_finalStateNormalization = new double[_finalStateNum];
	_balanceWeights = new double[_balanceNum];
	_balanceNormalization = new double[_balanceNum];
	_controlWeights = new double[_controlNum];
	_controlNormalization = new double[_controlNum];

	return true;
}

bool PSO::initialParticle(unsigned index, bool initialBest)
{
	for (unsigned j = 0; j < _particleDimNum; ++j)
	{
		for (unsigned m = 0; m < _particleDim[j]; ++m)
		{
			_particlePositions[index][j][m] = Math::random(_positionScope[j][0], _positionScope[j][1]);
			_particleVelcities[index][j][m] = Math::random(_velcityScope[j][0], _velcityScope[j][1]);
		}
	}
	getFitness(index);
	if (initialBest)
	{
		for (unsigned j = 0; j < _particleDimNum; ++j)
		{
			for (unsigned m = 0; m < _particleDim[j]; ++m)
			{
				_localBestPosition[index][j][m] = _particlePositions[index][j][m];
			}
		}
		_localBestFitness[index] = _fitness[index];
	}
	return true;
}

double PSO::getFitness(unsigned index)
{
	sendControl(index);

	assert(_stateNum == (_finalStateNum + _pathStateNum));
	double result = 0;
	// 路径约束 不等式约束
	for (unsigned i = 0; i < _pathStateNum; ++i)
	{
		unsigned tempMaxIndex = Math::max(_state[_finalStateNum + i], _stateDim[_finalStateNum + i]);
		result += _pathStateWeights[i] * (_pathStateMax[i] - _state[_finalStateNum + i][tempMaxIndex]) / _pathStateNormalization[i];
	}

	// 终端约束
	for (unsigned i = 0; i < _finalStateNum; ++i)
	{
		result -= _finalStateWeights[i] * (_state[i][_stateDim[i] - 1] - _finalState[i]) / _finalStateNormalization[i];
	}

	double *temp = new double[_controlNum];
	memset(temp, 0, sizeof(double)*_controlNum);
	// 控制变量约束
	for (unsigned i = 0; i < _controlNum; ++i)
	{
		for (unsigned j = 1; j < _particleDim[i]; ++j)
		{
			temp[i] += abs(_particlePositions[index][i][j] - _particlePositions[index][i][j - 1]);
		}
		result -= _controlWeights[i] * temp[i] / _controlNormalization[i];
	}
	delete temp;

	double *temp2 = new double[_balanceNum];
	memset(temp2, 0, _balanceNum * sizeof(double));
	// 拟平衡滑翔约束
	for (unsigned j = 1; j < _stateDim[4]; ++j)
	{
		temp2[0] += abs(_state[4][j]);
		temp2[1] += abs(_state[4][j] - _state[4][j - 1]);
	}
	for (unsigned i = 0; i < _balanceNum; ++i)
	{
		result -= _balanceWeights[i] * temp2[i] / _balanceNormalization[i];
	}
	delete temp2;

	if (result != result)
	{
		initialParticle(index);
	}

	return result;
}

double PSO::updateLocalBest(unsigned index)
{
	if (_fitness[index] > _localBestFitness[index])
	{
		_localBestFitness[index] = _fitness[index];
		for (unsigned j = 0; j < _particleDimNum; ++j)
		{
			for (unsigned m = 0; m < _particleDim[j]; ++m)
			{
				_localBestPosition[index][j][m] = _particlePositions[index][j][m];
			}
		}
	}
	return _fitness[index];
}

bool PSO::updateVelocity(unsigned index)
{
	for (unsigned j = 0; j < _particleDimNum; ++j)
	{
		for (unsigned m = 0; m < _particleDim[j]; ++m)
		{
			_particleVelcities[index][j][m] +=
				_inertiaWeight*_particleVelcities[index][j][m] +
				_learningFactor1*Math::random(0.0, 1.0)*(_bestPosition[j][m] - _particlePositions[index][j][m]) +
				_learningFactor2*Math::random(0.0, 1.0)*(_localBestPosition[index][j][m] - _particlePositions[index][j][m]);
			Math::keepRange(_particleVelcities[index][j][m], _velcityScope[j][0], _velcityScope[j][1]);
		}
	}
	return true;
}

bool PSO::updatePosition(unsigned index)
{
	for (unsigned j = 0; j < _particleDimNum; ++j)
	{
		for (unsigned m = 0; m < _particleDim[j]; ++m)
		{
			_particlePositions[index][j][m] += _particleVelcities[index][j][m];
			Math::keepRange(_particlePositions[index][j][m], _positionScope[j][0], _positionScope[j][1]);
		}
	}

	return true;
}

bool PSO::sendControl(unsigned index)
{
	Array3<double> mes(_particlePositions[index], _particleDim, _particleDimNum);
	_mediator->sendControl(mes);
	return true;
}

bool PSO::sendControl(double ** control)
{
	Array3<double> mes(control, _particleDim, _particleDimNum);
	_mediator->sendControl(mes);
	return true;
}

bool PSO::sendControl(double ** control, unsigned * dim, unsigned num)
{
	Array3<double> mes(control, dim, num);
	_mediator->sendControl(mes);
	return true;
}

bool PSO::recvData(Array3<double> state)
{
	_state = state._array;
	_stateNum = state._row;
	_stateDim = state._column;
	return false;
}

bool PSO::tieMediator(Mediator * mediator)
{
	_mediator = mediator;
	return true;
}

