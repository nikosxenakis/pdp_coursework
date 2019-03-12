#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include <iostream>
#include "actor.h"
#include "squirrel.h"
#include "clock.h"
#include "cell.h"
#include "input_data.h"

using namespace std;

class Actor_factory {

public:
	static int cell_number;

	static Actor* create(int id, int type, int master_pid, int worker_pid, int workers_num, void* v_data);
};

#endif
