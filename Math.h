#pragma once
#include<cmath>
#include<stdlib.h>

class Math
{
private:
	Math();
public:
	~Math();

private:
	static double _error;
public:
	static double SOUND_VELCITY;
	static double PI;
public:
	template<typename T>
	static T random(T min, T max);

	template<typename T>
	static unsigned max(T *data, unsigned num);

	static double linearInterpolation(double x1, double y1, double x2, double y2, double x);

	static double getDegree(double radian);
	static double getRadian(double degree);

	static bool keepRange(double &num, double min, double max);
};

template<typename T>
inline T Math::random(T min, T max)
{
	double decimals = static_cast<double>(rand()) / RAND_MAX;
	T result = (max - min)*decimals + min;
	return result;
}

template<typename T>
inline unsigned Math::max(T * data, unsigned num)
{
	unsigned index = 0;
	T max = data[index];
	for (unsigned i = 0; i < num; ++i)
	{
		if (data[index] > max)
		{
			max = data[index];
			index = i;
		}
	}
	return index;
}

//long double operator ""_deg(long double degree)
//{
//
//}