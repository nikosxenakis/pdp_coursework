#ifndef MESSENGER_H
#define MESSENGER_H

#include <mpi.h>
#include <iostream>
#include "message.h"

using namespace std;

class Messenger {

public:
	static MPI_Datatype Message_type;
	static void init_types();
	static void send_message(int worker_pid, Message message);
	static void send_blocking_message(int worker_pid, Message message);
	static Message receive_message(int source_pid);
};

#endif
