#ifndef SQUIRREL_H
#define SQUIRREL_H

#include <iostream>
#include "actor.h"

using namespace std;

class Squirrel: public Actor
{

public:
	Squirrel(int id, int master_pid, int worker_pid);
	void compute() override;
	void parse_message(Message message) override;

	void visited(int actor_id);
};

#endif
