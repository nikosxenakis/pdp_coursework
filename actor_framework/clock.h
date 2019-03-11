#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include "actor.h"

using namespace std;

class Clock: public Actor {

public:
	vector<Actor*> squirrels;
	vector<Actor*> cells;

	int max_months;
	int timestep;
	int births;
	int births_came;

	int cells_ready;
	int squirrels_ready;

	Clock(int id, int master_pid, int worker_pid, int max_months);
};

#endif
