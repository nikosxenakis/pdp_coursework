#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <string>
#include <vector>

#include "actor.h"
#include "message.h"
#include "messenger.h"

using namespace std;

class Master
{
private:

public:
	static int pid;
	static int workers_num;
	static int next_actor_id;
	static vector<int> workers_pid;
	static void* input_data;
	static int dead_workers;
	
	static Actor* (*create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data);

	static void init_workers(vector<int> workers_pid);
	static void register_create_actor(Actor* (*)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data), void *data);
	static void initialize_master(int pid, int workers_num);
	static int get_next_worker();
	static void spawn_actor(Message message);
	static void start_simulation();
	static void run();
	static int parse_message(int source_pid, Message message);
	static void finalize();
	static void print();
};

#endif
