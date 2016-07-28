#pragma once

#include "Array3.h"

class Algorithm
{
public:
	Algorithm();
	virtual ~Algorithm();

public:
	virtual bool calculate() = 0;
};

