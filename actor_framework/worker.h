#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <vector>
#include "actor.h"
#include "message.h"
#include "messenger.h"

using namespace std;

#define MASTER_PID 0

/**
 * @class Worker
 * @details class containing information and functionality for the Worker process
 */
class Worker {

private:
	/**
	 * @brief worker process number
	 */
	int pid;

	/**
	 * @brief vector that contains pointers to the actors that live in this worker
	 */
	vector<Actor*> actors;

	/**
	 * @brief indicates if the simulation should start or not
	 */
	bool start_simulation;

	/**
	 * @brief function pointer to the given callback that spawns actors
	 */
	static Actor* (*spawn_actor)(Message message);

	/**
	 * @brief number of spawned actors in this worker
	 */
	int actors_spawned;

	/**
	 * @brief number of died actors in this worker
	 */
	int actors_died;

	static Message input_data;

	void add_actor(Actor *actor);
	void remove_actor(int actor_id);
	Actor* find_actor(int actor_id);
	void kill_all_actors();

	void compute();
	int process(Message message);

public:

	Worker(int pid);
	~Worker();
	static void register_spawn_actor(Actor* (spawn_actor)(Message message), Message message);
	void run();
	void finalize();
};

#endif
