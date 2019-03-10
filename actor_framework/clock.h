#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include "actor.h"

using namespace std;

class Clock: public Actor {

public:
	vector<Actor*> squirrels;
	int max_months;
	int timestep;
	int squirrels_finished_timestep;
	int births;

	Clock(int id, int master_pid, int worker_pid, int max_months);
};

#endif
