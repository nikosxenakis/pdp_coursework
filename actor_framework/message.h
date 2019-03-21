#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <string>
#include "framework_commands.h"
#include "framework_message_types.h"

class Message {
private:
	float message_data[MESSAGE_SIZE];
public:
	friend class Messenger;
	Message();
	void print();
	float get(int pos);
	void set(int pos, float data);
};

#endif
