#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <iostream>
#include <string>
#include "commands.h"

#define MESSAGE_SIZE 17

#define COMMAND 0
#define ACTOR_TYPE 1
#define ACTOR_ID 2
#define ACTOR_ID_DEST 3
#define WORKERS_NUM 4
#define WORKER_PID 5
#define INIT_ACTORS_NUM 6
#define MAX_ACTORS_NUM 7

using namespace std;

class Message {

public:
	float message_data[MESSAGE_SIZE];

	Message();
	// Message(float message_data[MESSAGE_SIZE]);
	void print();
	string get_string_command();
	void set(int pos, float data);
	float get(int pos);
	string get_field_name(int pos);

};

#endif
