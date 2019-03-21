#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "actor_framework_interface.h"
#include "actor_types.h"
#include "simulation_message_types.h"
#include "simulation_commands.h"

#define VIRUS_LIFETIME 2

using namespace std;

class Cell: public Actor {

public:
	int max_months;
	int timestep;
	int population_influx;
	vector<int> population_in_steps;
	int infection_level;
	vector<int> inflection_in_steps;
	vector<int> alive_squirrels;
	vector<int> infected_squirrels;

	Cell(int id, int master_pid, int worker_pid, int workers_num, int max_months);
	~Cell();

	void visited(Message message);

};

#endif
