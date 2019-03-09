#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <iostream>
#include <string>
#include "commands.h"

using namespace std;

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
	string get_string_command();
};

#endif
