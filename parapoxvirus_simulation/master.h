#ifndef MASTER_H
#define MASTER_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>

#include "worker.h"
#include "pool.h"
#include "actor.h"
#include "message.h"
#include "messenger.h"

using namespace std;

class Master
{
private:
	static void init_workers();

public:
	static int pid;
	static int workers_num;
	static int active_actors;
	static vector<Worker*> workers;

	static void initialize_master(int pid, int workers_num);
	static void spawn_actor(Actor *actor);
	static Worker* find_available_worker();
	static Actor* find_actor(int id);
	static void run();
	static void parse_message(Message message);
	static void kill_actor(int actor_id);
	static void finalize();
	static void print();
};

#endif
