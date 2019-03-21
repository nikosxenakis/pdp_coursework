#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include <map>
#include <assert.h>
#include "message.h"
#include "messenger.h"

#define MASTER_PID 0

using namespace std;

/**
 * @class Actor
 * @brief Base abstract class of Actors
 * @details A class that is inhereted by Actors in the user simulation poviding a mechanism to communicate with other actors and operate
 */
class Actor {

private:
	/**
	 * @brief unique identification of actor
	 */
	int id;

	/**
	 * @brief number of active worker processes
	 */
	int workers_num;

	/**
	 * @brief current state of Actor in FSM
	 */
	int state;

	/**
	 * @brief type of Actor it is described in the simulation during the construction of the derived object
	 */
	int type;

	/**
	 * @brief map data structure that contains a compute Actor function for a given state in FSM
	 */
	map<int, void (*)(Actor*)> compute_map;

	/**
	 * @brief map data structure that contains a parse message Actor function for a given state in FSM
	 */
	map<int, void (*)(Actor*, Message)> parse_message_map;

	/**
	 * @brief Find the worker pid that the given actor id belongs
	 * @param actor_id Actor id
	 * @return Returns the worker pid
	 */
	int get_worker(int actor_id);

protected:

	/**
	 * @brief Adds the given compute function to the compute_map based on the given state in FSM
	 * @param state FSM state
	 * @param compute_f static compute function pointer
	 */
	void register_state(int state, void (compute_f)(Actor*));

	/**
	 * @brief Adds the given parse message function to the compute_map based on the given state in FSM
	 * @param state FSM state
	 * @param compute_f static parse message function pointer
	 */
	void register_state(int state, void (parse_message_f)(Actor*, Message));

	/**
	 * @brief Sends message to the master to create a new Actor
	 * @param message The input message containing information about the new Actor
	 */
	void create_actor(Message message);

	/**
	 * @brief Sends message to the master and the appropriate worker(this worker) to kill this actor
	 */
	void kill_actor();

	/**
	 * @brief Sends message to the master to kill all actors, after this call the simulation is finilised
	 */
	void kill_all();

public:

	/**
	 * @brief Actor constructor
	 * @param id Actor id
	 * @param type Actor type
	 * @param workers_num number of active worker processes
	 */
	Actor(int id, int type, int workers_num);

	/**
	 * @brief Actor destructor
	 */
	virtual ~Actor() = 0;

	/**
	 * @return Returns the Actor id
	 */
	int get_id();

	/**
	 * @brief Sets new FSM state to the Actor
	 * @param state FSM state
	 */
	void set_state(int state);

	/**
	 * @brief Executes the compute function of the Actor based on its current state
	 */
	void compute();

	/**
	 * @brief Executes the parse message function of the Actor based on its current state
	 * @param message The input message for Actor
	 */
	void process(Message message);

	/**
	 * @brief Send the given message to the specified actor_id
	 * @param actor_id The receiver actor id
	 * @param message The message to send
	 */
	void send_msg(int actor_id, Message message);
};

#endif
