#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include "actor.h"
#include "squirrel.h"

using namespace std;

class Actor_factory
{

public:
	static Actor* create(int id, int type, int master_pid);
	static Actor* create(int type);
};

#endif
