#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include "actor.h"
#include "squirrel.h"
#include "clock.h"
#include "cell.h"

using namespace std;

class Actor_factory
{

public:
	static Actor* create(int id, int type, int master_pid, int worker_pid);
};

#endif
