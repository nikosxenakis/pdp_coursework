#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <iostream>
#include "actor.h"

using namespace std;

#define SPAWN_ACTOR_COMMAND 0

class Message
{

public:
	int command;
	int actor_id;
	int actor_type;

	Message(int command, Actor *actor);
	void print();
};

#endif
