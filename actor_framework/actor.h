#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include "message.h"
#include "messenger.h"

using namespace std;

#define COMPUTE 0
#define PARSE_MESSAGE 1

class Actor {

private:
	int id;
	int master_pid;
	int worker_pid;
	int workers_num;
	int state;
	int type;
	map<int, void (*)(Actor*)> compute_map;
	map<int, void (*)(Actor*, Message)> parse_message_map;

	void set_worker(int worker_pid);

	int get_worker(int actor_id);

protected:

	void register_state(int type, int state, void (f)(Actor*));
	void register_state(int type, int state, void (f)(Actor*, Message));

public:

	Actor(int id, int master_pid, int worker_pid, int workers_num);
	virtual ~Actor();

	int get_type();
	int get_id();

	void set_type(int type);
	void set_state(int state);

	void print();

	void create_actor(Message message);
	void kill_actor();
	void kill_all();

	void compute();
	void process(Message message);
	void send_msg(int actor_id, Message message);
};

#endif
