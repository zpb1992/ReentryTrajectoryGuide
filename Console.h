#pragma once
#include<iostream>

class Console
{
private:
	Console();
public:
	~Console();
private:
	static Console *_console;
public:
	static Console *instance();

	void error(std::string mes);
	Console * warn(std::string mes);
	Console * inform(std::string mes);

	void error(int num);
	Console * warn(int num);
	Console * inform(int num);

	Console * count(int start,bool initial = false);
};

