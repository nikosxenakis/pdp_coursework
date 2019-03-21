#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <string>

#include "message.h"
#include "messenger.h"

using namespace std;

class Master {

private:

	/**
	 * @brief number of active worker processes
	 */
	static int workers_num;

	/**
	 * @brief unique id for the next actor to be spawned
	 */
	static int next_actor_id;

	static void* input_data;
	static int dead_workers;
	static int actors_spawned;
	static int actors_died;

	static int get_next_worker();
	static void start_simulation();
	static int parse_message(int source_pid, Message message);

public:

	static void initialize_master(int workers_num);
	static void spawn_actor(Message message);
	static void run();
	static void finalize();
	static void print();
};

#endif
