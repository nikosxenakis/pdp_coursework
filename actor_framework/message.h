#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <iostream>
#include <string>
#include "commands.h"

using namespace std;

typedef struct Message_data {
	int command;
	int actor_id;
	int actor_id_dest;
	int actor_type;
	int worker_pid;
	int healthy;
	int population_influx;
	int infection_level;
	int alive_squirrels;
	int infected_squirrels;
	float x;
	float y;
} Message_data;

class Message {

public:
	Message_data message_data;

	Message();
	Message(Message_data message_data);
	// Message(int command, int actor_type);
	// Message(int command, int actor_id, int actor_type);
	// Message(int command, int actor_id, int actor_type, int worker_pid);
	void print();
	string get_string_command();
};

#endif
