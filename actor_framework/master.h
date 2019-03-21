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

	/**
	 * @brief number of actors died during the simulation
	 */
	static int actors_died;

	/**
	 * @brief Find the worker pid that the next actor will belong
	 * @return Returns the worker pid
	 */
	static int get_next_worker();

	/**
	 * @brief Notifies the workers to start and starts the simulation
	 */
	static void start_simulation();

	/**
	 * @brief Parses the given message and does the appropriate functionality
	 * @param source_pid Pid of the source process
	 * @param message Input message for processing
	 * @return Returns 1 of the simulation should exit otherwise it returns 0
	 */
	static int parse_message(int source_pid, Message message);

public:

	/**
	 * @brief Sets the number of active worker processes
	 * @param workers_num number of active worker processes
	 */
	static void set_workers_num(int workers_num);

	/**
	 * @brief Send message to a worker to spawn a new actor
	 * @param message Message for the creation of the new actor
	 */
	static void spawn_actor(Message message);

	/**
	 * @brief Runs the main master code waiting for messages
	 */
	static void run();

	/**
	 * @brief Finalises the master process by sending finalisation messages to the workers
	 */
	static void finalize();
};

#endif
