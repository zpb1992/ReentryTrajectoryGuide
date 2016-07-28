#pragma once

#include "Array3.h"

class Problem
{
public:
	Problem();
	~Problem();
	
public:
	virtual bool calculateState()=0;
};

