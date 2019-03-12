#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <chrono>
#include "actor.h"

using namespace std;
using namespace std::chrono;

class Clock: public Actor {

public:
	vector<int> population_influx;
	vector<int> infection_level;
	milliseconds begin_time;
	int max_months;
	int timestep;
	int cells_ready;

	Clock(int id, int master_pid, int worker_pid, int workers_num, int max_months);
};

#endif
