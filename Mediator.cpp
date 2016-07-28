#include "Mediator.h"


Mediator::Mediator(Component * problem, Component * algorithm)
{
	_problem = problem;
	_algorithm = algorithm;
}

Mediator::~Mediator()
{
}

bool Mediator::sendControl(Array3<double> &control)
{
	_problem->recvData(control);
	return true;
}

bool Mediator::sendState(Array3<double> &state)
{
	_algorithm->recvData(state);
	return true;
}

