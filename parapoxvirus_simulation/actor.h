#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include "actor_types.h"
#include "message.h"
#include "messenger.h"

using namespace std;

// typedef enum Actor_state {
// 	STATE_0 = 0,
// 	STATE_1,
// 	STATE_2
// } Actor_state;

// static void check_messages();
// static void process_message();

class Actor
{

protected:
	int id;
	int master_pid;
	int type;

public:
	Actor(int id, int master_pid);
	int get_id();
	void print();
	int get_type();
	virtual void compute() = 0;
	void create_actor(int actor_type);
	void die();
};

#endif
