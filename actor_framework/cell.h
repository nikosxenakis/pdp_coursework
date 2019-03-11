#ifndef CELL_H
#define CELL_H

#include <iostream>
#include "actor.h"

#define VIRUS_LIFETIME 2

using namespace std;

class Cell: public Actor {

public:
	// vector<Actor*> squirrels;
	int cell_number;
	int healthy;
	int virus_age;
	int timestep;
	int population_influx; //for past 3 months
	int infection_level; //for past 2 months
	Actor *clock;

	Cell(int id, int master_pid, int worker_pid, int cell_number);

	void visited(Message message);

};

#endif
