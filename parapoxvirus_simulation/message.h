#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <iostream>
#include <string>

using namespace std;

#define SPAWN_ACTOR_COMMAND 0
#define KILL_ACTOR_COMMAND 1

#define KILL_WORKER_COMMAND 10

class Message
{

public:
	int command;
	int actor_id;
	int actor_type;

	Message();
	Message(int command);
	Message(int command, int actor_id, int actor_type);
	void print();
	string get_string_command(int command);
};

#endif
