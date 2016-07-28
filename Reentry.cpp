#include "Reentry.h"
#include "json\json.h"
#include<fstream>
#include<cmath>
#include"Math.h"
#include<cassert>

Reentry::Reentry(std::string filename) :Configuration(filename)
{
	initial(_fileName);
	readConfig(_fileName);
}

Reentry::~Reentry()
{
	if (_state != nullptr)
	{
		for (unsigned i = 0; i < _stateNum; ++i)
		{
			delete _state[i];
		}
		delete _state;
		_state = nullptr;
	}
	//if (_mediator != nullptr)
	//{
	//	delete _mediator;
	//	_mediator = nullptr;
	//}
}

bool Reentry::readConfig(std::string filename)
{
	Json::Reader reader;
	std::ifstream ifs(filename);
	Json::Value root;
	reader.parse(ifs, root);

	_stateDim = root[getKeyByVar(ToString(_stateDim))].asUInt();
	_state = new double *[_stateNum + _pathContraintsNum];
	for (unsigned i = 0; i < (_stateNum + _pathContraintsNum); ++i)
	{
		_state[i] = new double[_stateDim];
		if (1 == i || 2 == i || 4 == i || 5 == i)
		{
			_state[i][0] = Math::getRadian(root[getKeyByVar(ToString(_state))][i].asDouble());
		}
		else
		{
			_state[i][0] = root[getKeyByVar(ToString(_state))][i].asDouble();
		}
	}
	_c1 = root[getKeyByVar(ToString(_c1))].asDouble();
	_vc = root[getKeyByVar(ToString(_vc))].asDouble();

	return true;
}

bool Reentry::writeConfig(std::string filename)
{
	return false;
}

bool Reentry::initial(std::string filename)
{
	Json::Reader reader;
	std::ifstream ifs(filename);
	Json::Value root;
	reader.parse(ifs, root);

	_stateNum = root[getKeyByVar(ToString(_stateNum))].asUInt();
	_pathContraintsNum = root[getKeyByVar(ToString(_pathContraintsNum))].asUInt();
	return true;
}

bool Reentry::sendState()
{
	Array3<double> mes(_state, _stateDim, (_stateNum + _pathContraintsNum));
	_mediator->sendState(mes);
	return true;
}

bool Reentry::sendState(double ** state)
{
	Array3<double> mes(state, _stateDim, (_stateNum + _pathContraintsNum));
	_mediator->sendState(mes);
	return true;
}

bool Reentry::sendState(double ** state, unsigned dim, unsigned num)
{
	Array3<double> mes(state, dim, num);
	_mediator->sendState(mes);
	return true;
}

bool Reentry::recvData(Array3<double> control)
{
	_control = control._array;
	_controlDim = control._column;
	_controlNum = control._row;

	calculateState();

	sendState();

	return true;
}

bool Reentry::calculateState()
{
	LGKT4();
	getPathState();
	return true;
}

bool Reentry::LGKT4()
{
	for (unsigned j = 0; j < (_stateDim - 1); ++j)
	{
		double control[2];
		double **k = new double *[4];
		double delta = 1.0 / (_stateDim - 1);
		double realDeltaTime = delta*_control[2][0];
		double *tempState = new double[_stateNum];
		for (int i = 0; i < 4; ++i)
		{
			k[i] = new double[_stateNum];
		}

		for (unsigned i = 0; i < _stateNum; ++i)
			tempState[i] = _state[i][j];
		getControls(j / _stateDim, control);
		DiffFunc(control, tempState, k[0]);
		for (unsigned i = 0; i < _stateNum; ++i)
			tempState[i] = _state[i][j] + realDeltaTime*k[0][i] / 2.0;
		getControls(j / _stateDim + delta / 2, control);
		DiffFunc(control, tempState, k[1]);
		for (unsigned i = 0; i < _stateNum; ++i)
			tempState[i] = _state[i][j] + realDeltaTime*k[1][i] / 2.0;
		//getControls(j / _stateDim + delta / 2, control);
		DiffFunc(control, tempState, k[2]);
		for (unsigned i = 0; i < _stateNum; ++i)
			tempState[i] = _state[i][j] + realDeltaTime*k[2][i];
		getControls(j / _stateDim + delta, control);
		DiffFunc(control, tempState, k[3]);

		for (unsigned i = 0; i < _stateNum; ++i)
		{
			_state[i][j + 1] = _state[i][j] + realDeltaTime*(k[0][i] + k[1][i] + k[2][i] + k[3][i]) / 6.0;
		}

		for (int i = 0; i < 4; ++i)
		{
			delete k[i];
			k[i] = nullptr;
		}
		delete k;
		k = nullptr;
	}
	return true;
}

bool Reentry::DiffFunc(double * control, unsigned stateIndex, double * result)
{
	double *state = new double[_stateNum];
	for (unsigned i = 0; i < _stateNum; ++i)
	{
		state[i] = _state[i][stateIndex];
	}
	DiffFunc(control, state, result);
	return true;
}

bool Reentry::DiffFunc(double * control, double * state, double * result)
{
	double s5 = sin(state[4]);
	double c5 = cos(state[4]);
	double s6 = sin(state[5]);
	double c6 = cos(state[5]);
	double s3 = sin(state[2]);
	double c3 = cos(state[2]);

	double Cl = getCl(control[1], state[3]);
	double Cd = getCd(control[1], state[3]);

	double L = getLift(_atmosphere.getAirDensity(state[0] - _star.getRadius()), state[3], _aircraft.getS(), Cl);
	double D = getDrag(_atmosphere.getAirDensity(state[0] - _star.getRadius()), state[3], _aircraft.getS(), Cd);

	double g = _star.getGravityAcc(state[0]);
	double wE = _star.getRotationW();

	//量纲方程  所有量都为量纲量
	result[0] = state[3] * s5;
	result[1] = state[3] * c5*s6 / (state[0] * c3);
	result[2] = state[3] * c5*c6 / state[0];
	result[3] = -D / _aircraft.getM() - g*s5 + wE*wE*state[0] * c3*(s5*c3 - c5*s3*c6);
	result[4] = (L*cos(control[0]) / _aircraft.getM() + (state[3] * state[3] / state[0] - g)*c5 + 2 * wE*state[3] * c3*s6 + wE*wE*state[0] * c3*(c5*c3 + s5*s3*c6)) / state[3];
	result[5] = (L*sin(control[1]) / (_aircraft.getM()*c5) + state[3] * state[3] / state[0] * c5*s6*tan(state[2]) - 2 * wE*state[3] * (c3*tan(state[4])*c6 - s3) + wE*wE*state[0] / c5*s3*c3*s6) / state[3];
	return true;
}

bool Reentry::getPathState()
{
	for (unsigned j = 0; j < _stateDim; ++j)
	{
		_state[_stateNum][j] = heatFlux(j);
		_state[_stateNum + 1][j] = dynamicPressure(j);
		_state[_stateNum + 2][j] = overload(j);
	}
	return true;
}

double Reentry::heatFlux(unsigned index)
{
	double density = _atmosphere.getAirDensity(_state[0][index]);
	return _c1 / pow(_aircraft.getR(), 0.5)*pow(density / _atmosphere.getAirDensity(), 0.5)*pow(_state[3][index] / _vc, 3.15);
}

double Reentry::dynamicPressure(unsigned index)
{
	double density = _atmosphere.getAirDensity(_state[0][index]);
	return density*_state[3][index] * _state[3][index] / 2;
}

double Reentry::overload(unsigned index)
{
	double attack = getOneControl(index / (_stateDim - 1), _control[1], _controlDim[1]);
	double Cl = getCl(attack, _state[3][index]);
	double Cd = getCd(attack, _state[3][index]);
	return _state[7][index] * pow((Cl*Cl + Cd*Cd), 0.5)*_aircraft.getS() / (_aircraft.getM()*_star.getGravityAcc(_state[0][index]));
}

double Reentry::getCl(double attack, double velcity)
{
	return 2.9451*attack + 0.2949*exp(-0.00033943*velcity / Math::SOUND_VELCITY) - 0.2355;
}

double Reentry::getCd(double attack, double velcity)
{
	return 2.3795*attack*attack + 0.3983*exp(-0.0010794*velcity / Math::SOUND_VELCITY) + 0.0234;
}

double Reentry::getLift(double airDensity, double velcity, double aircraftS, double Cl)
{
	return airDensity*velcity*velcity*aircraftS*Cl / 2;;
}

double Reentry::getDrag(double airDensity, double velcity, double aircraftS, double Cd)
{
	return airDensity*velcity*velcity*aircraftS*Cd / 2;
}

bool Reentry::getControls(double time, double * result, unsigned num)
{
	assert(time<1.001&&time>-0.001);

	double delta = 1.0 / _controlDim[0];
	double startTime = 0.0;
	unsigned startIndex = 0;
	while (time < startTime)
	{
		startTime += delta;
		++startIndex;
	}
	assert(startTime<(1.0 + delta + 0.001) && startTime>(-delta - 0.001));

	result[0] = Math::linearInterpolation(startTime, _control[0][startIndex], (startTime + delta), _control[0][startIndex + 1], time);
	result[1] = Math::linearInterpolation(startTime, _control[1][startIndex], (startTime + delta), _control[1][startIndex + 1], time);

	return true;
}

double Reentry::getOneControl(double time, double * control, unsigned dim)
{
	assert(time<1.001&&time>-0.001);

	double delta = 1.0 / dim;
	double startTime = 0.0;
	unsigned startIndex = 0;
	while (time < startTime || time>(startTime + delta))
	{
		startTime += delta;
		++startIndex;
	}
	assert(startTime<(1.0 + delta + 0.001) && startTime>(-delta - 0.001));

	return Math::linearInterpolation(startTime, control[startIndex], (startTime + delta), control[startIndex + 1], time);
}

bool Reentry::tieMediator(Mediator * mediator)
{
	_mediator = mediator;
	return true;
}

