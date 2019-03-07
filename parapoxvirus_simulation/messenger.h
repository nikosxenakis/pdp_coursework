#ifndef MESSENGER_H
#define MESSENGER_H

#include <mpi.h>
#include <iostream>
#include "actor.h"
#include "message.h"

using namespace std;

class Messenger
{

public:
	static void send_message(int worker_pid, Message message);
	// static Message read_message(Message message);
};

#endif
