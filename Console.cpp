#include "Console.h"
#include<string>

Console *Console::_console = nullptr;

Console::Console()
{
}

Console::~Console()
{
}

Console * Console::instance()
{
	if (_console == nullptr)
	{
		_console = new Console();
	}
	return _console;
}

void Console::error(std::string mes)
{
	std::cout << "\033[31m"
		<< mes
		<< "\033[0m" << std::endl;
	exit(1);
}

Console * Console::warn(std::string mes)
{
	std::cout << "\033[33m"
		<< mes
		<< "\033[0m" << std::endl;
	return this;
}

Console * Console::inform(std::string mes)
{
	std::cout << "\033[32m"
		<< mes
		<< "\033[0m" << std::endl;
	return this;
}

void Console::error(int num)
{
	std::cout << "\033[31m"
		<< "count: " << num
		<< "\033[0m" << std::endl;
	exit(1);

}

Console * Console::warn(int num)
{
	std::cout << "\033[33m"
		<< "count: " << num
		<< "\033[0m" << std::endl;
	return this;
}

Console * Console::inform(int num)
{
	std::cout << "\033[32m"
		<< "count: " << num
		<< "\033[0m" << std::endl;
	return this;
}

Console * Console::count(int start, bool initial)
{
	static int num = 0;
	if (initial)
		num = start;	
	std::cout << "\033[32m"
		<< "count: " << num
		<< "\033[0m" << std::endl;
	++num;

	return this;
}

