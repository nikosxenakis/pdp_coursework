#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <iostream>
#include <string>
#include "framework_commands.h"
#include "framework_message_types.h"

using namespace std;

class Message {

public:
	float message_data[MESSAGE_SIZE];

	Message();
	void print();
	string get_string_command();
	void set(int pos, float data);
	float get(int pos);
	string get_field_name(int pos);
};

#endif
