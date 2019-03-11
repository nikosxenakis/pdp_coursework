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
	Actor *clock;
	int timestep = 0;
	float x;
	float y;
	int healthy;
	int step_no;
	vector<int> steps;
	long seed;
	int infected_steps;

	Squirrel(int id, int master_pid, int worker_pid, float x, float y);
	void print();
	int move();
	void visit(int actor_id);
	int birth();
	int will_die();
};

#endif
