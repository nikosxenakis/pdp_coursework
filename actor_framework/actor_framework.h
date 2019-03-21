#ifndef ACTOR_FRAMEWORK_H
#define ACTOR_FRAMEWORK_H

#include <mpi.h>
#include "master.h"
#include "worker.h"
#include "messenger.h"

#define MASTER_PID 0
#define UPPER_BOUND_BUFFER_SIZE 10000

/**
 * @class Actor_framework
 * @brief Primary interface of actor framework
 * @details A class that provides a set of functions for the simulation to initialise the framework
 */
class Actor_framework {

private:

	/**
	 * @brief pointer to a provided callback function that initialises the actors in the simulation based on the data in the given message
	 */
	static void (*init_actors)(Message message);

	/**
	 * @brief create a new worker object and executes its run function
	 * @param pid worker process pid
	 * @param message a message containing vital infromation
	 */
	static void worker_code(int pid, Message message);

	/**
	 * @brief executes the run function of master process
	 * @param pid worker process pid
	 * @param message a message containing vital infromation
	 */
	static void master_code(int pid, Message message);

public:

	/**
	 * @brief registers the initialisation actors callback
	 * @param init_actors pointer to a provided callback function that initialises the actors in the simulation based on the data in the given message
	 */
	static void register_init_actors(void (init_actors)(Message message));

	/**
	 * @brief registers the initialisation actors callback
	 * @param spawn_actor pointer to a provided callback function that Master process uses to spawn new actors
	 */
 	static void register_spawn_actor(Actor* (spawn_actor)(Message message), Message message);

	/**
	 * @brief calls the spawn actor callback function in the Master process
	 * @param message data for the creation of the new actor
	 */
	static void spawn_actor(Message message);

	/**
	 * @brief initialises the MPI process setting up the environment
	 * @param message data containing the input arguments of the process
	 */
	static void actor_framework(Message message);
};

#endif
