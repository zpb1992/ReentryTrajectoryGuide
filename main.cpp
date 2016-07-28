#include"Console.h"
#include "PSO.h"
#include "Reentry.h"
#include "Mediator.h"
#include<ctime>

#pragma comment(lib,"./Debug/jsoncpp.lib")

int main()
{
	srand(static_cast<int>(time(0)));


	PSO pso;
	Reentry reentry;
	Mediator mediator(&reentry, &pso);

	unsigned start = time(0);


	pso.tieMediator(&mediator);
	reentry.tieMediator(&mediator);

	pso.calculate();

	unsigned end = time(0);

	Console::instance()->inform("run time")->inform(end-start);

	return 0;
}