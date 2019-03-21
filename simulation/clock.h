#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>
#include <assert.h>
#include "actor_framework_interface.h"
#include "actor_types.h"
#include "simulation_message_types.h"
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
	static stringstream population_influx_stream;
	static stringstream infection_level_stream;
	int alive_squirrels;
	int infected_squirrels;

	void write_output_stream();
	void write_output_files();

	Clock(int id, int workers_num, int max_months, int init_squirrels_no, int init_inf_squirrels_no);
	~Clock();

};

#endif
