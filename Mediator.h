#pragma once

#include "Problem.h"
#include "Algorithm.h"
#include "Array3.h"
#include "Component.h"

class Mediator
{
public:
	Mediator(Component *problem, Component *algorithm);
	~Mediator();

private:
	Component *_problem;
	Component *_algorithm;

public:		
	bool sendControl(Array3<double> &control);
	bool sendState(Array3<double> &state);
};

