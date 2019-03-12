#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <vector>
#include "actor.h"

#define VIRUS_LIFETIME 2

using namespace std;

class Cell: public Actor {

public:
	// vector<Actor*> squirrels;
	int cell_number;
	int max_months;
	int timestep;
	int population_influx;
	vector<int> population_in_steps;
	int infection_level;
	vector<int> inflection_in_steps;
	Actor *clock;

	Cell(int id, int master_pid, int worker_pid, int workers_num, int cell_number, int max_months);

	void visited(Message message);

};

#endif
