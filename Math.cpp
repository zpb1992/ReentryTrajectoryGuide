#include "Math.h"
#include<cassert>

double Math::_error= 0.001;
double Math::PI=3.1415926;
double Math::SOUND_VELCITY= 295.188;

Math::Math()
{
}


Math::~Math()
{
}

double Math::linearInterpolation(double x1, double y1, double x2, double y2, double x)
{
	assert(x1<(x + _error) && x2>(x - _error));
	return (y2-y1)*(x-x1)/(x2-x1)+y1;
}

double Math::getDegree(double radian)
{
	return radian/PI*180.0;
}

double Math::getRadian(double degree)
{
	return degree/180.0*PI;
}

bool Math::keepRange(double &num, double min, double max)
{
	if (num < min)
		num = min;
	else if (num > max)
		num = max;
	else
		return true;
	return false;
}

