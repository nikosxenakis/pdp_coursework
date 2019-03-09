#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include "actor.h"

using namespace std;

class Clock: public Actor {

public:
	vector<Actor*> squirrels;

	Clock(int id, int master_pid, int worker_pid);
};

#endif
