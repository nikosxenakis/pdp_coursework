#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>
#include "actor.h"
#include "actor_types.h"
#include "message_types.h"
#include "simulation_commands.h"

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
	// int alive_squirrels;
	// int infected_squirrels;
	static stringstream population_influx_stream;
	static stringstream infection_level_stream;

	void write_output_stream();
	void write_output_files();

	Clock(int id, int master_pid, int worker_pid, int workers_num, int max_months);
};

#endif
