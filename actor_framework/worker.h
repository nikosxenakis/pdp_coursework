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
 * @brief class containing information and functionality for the Worker process
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

	/**
	 * @brief add a new actor
	 * @param pointer to the new actor structure
	 */
	void add_actor(Actor *actor);

	/**
	 * @brief removes the requested actor
	 * @param id of the actor to kill
	 */
	void remove_actor(int actor_id);

	/**
	 * @brief finds the requested actor
	 * @param id of the actor to find
	 * @return Returns pointer to the requested actor
	 */
	Actor* find_actor(int actor_id);

	/**
	 * @brief kill all the actors in the worker
	 */
	void kill_all_actors();

	/**
	 * @brief Executes the compute function of the worker
	 */
	void compute();

	/**
	 * @brief Executes the parse message function of the worker
	 * @param message The input message for worker
	 */
	int process(Message message);

public:

	/**
	 * @brief Worker constructor
	 * @param pid process id of worker
	 */
	Worker(int pid);

	/**
	 * @brief Worker destructor
	 */
	~Worker();

	/**
	 * @brief registers the spawn actor function
	 * @param function pointer to the given callback that spawns actors
	 */
	static void register_spawn_actor(Actor* (spawn_actor)(Message message));

	/**
	 * @brief main functionality of worker
	 */
	void run();

	/**
	 * @brief finalize method of worker
	 */
	void finalize();
};

#endif
