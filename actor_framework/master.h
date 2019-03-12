#ifndef MASTER_H
#define MASTER_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>

#include "pool.h"
#include "worker.h"
#include "actor.h"
#include "message.h"
#include "messenger.h"
#include "input_data.h"

using namespace std;

class Master
{
private:
	static void init_workers();

public:
	static int pid;
	static int workers_num;
	static int max_actors_num;
	static int active_actors;
	static int next_actor_id;
	static vector<Worker*> workers;
	static void* input_data;
	static int dead_workers;
	
	static Actor* (*create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data);

	static void register_create_actor(Actor* (*)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data), void *data);
	static void initialize_master(int pid, int workers_num, int max_actors_num);
	static int get_next_worker();
	static void spawn_actor(Message message);
	// static Worker* find_available_worker();
	static Actor* find_actor(int id);
	static void start_simulation();
	static void run();
	static int compute();
	static int parse_message(int source_pid, Message message);
	static void kill_actor(int actor_id);
	static void finalize();
	static void print();
};

#endif
