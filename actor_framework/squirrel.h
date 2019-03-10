#ifndef SQUIRREL_H
#define SQUIRREL_H

#include <iostream>
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
	int steps;
	long seed;

	Squirrel(int id, int master_pid, int worker_pid, float x, float y);
	void print();
	void visited(int actor_id);
	void visit(int actor_id);
};

#endif
