#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <iostream>
#include <string>

using namespace std;

#define SPAWN_ACTOR_COMMAND 0
#define KILL_ACTOR_COMMAND 1
#define DISCOVER_ACTOR_COMMAND 2
#define KILL_WORKER_COMMAND 10

#define VISIT_ACTOR_COMMAND 20

class Message
{

public:
	int command;
	int actor_id;
	int actor_id_dest;
	int actor_type;
	int worker_pid;

	Message();
	Message(int command);
	Message(int command, int actor_type);
	Message(int command, int actor_id, int actor_type);
	Message(int command, int actor_id, int actor_type, int worker_pid);
	void print();
	string get_string_command(int command);
};

#endif
