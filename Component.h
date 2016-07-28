#pragma once

#include "Array3.h"
class Mediator;

class Component
{
public:
	Component();
	~Component();
public:
	virtual bool tieMediator(Mediator *mediator) = 0;
	virtual bool recvData(Array3<double> data) = 0;
};
