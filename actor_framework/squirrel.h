#ifndef SQUIRREL_H
#define SQUIRREL_H

#include <iostream>
#include <vector>
#include "actor.h"
#include "squirrel-functions.h"
#include "actor_framework.h"

using namespace std;

class Squirrel: public Actor {

public:
	float x;
	float y;
	int healthy;
	int step_no;
	vector<int> steps;
	long seed;
	int infected_steps;
	int counter;

	vector<int> population_influx;
	vector<int> infection_level;

	Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y);
	Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y, int healthy);
	void init(float x, float y, int healthy);

	void print();
	void move();
	void visit(int actor_id);
	void birth();
	void will_die();
};

#endif
