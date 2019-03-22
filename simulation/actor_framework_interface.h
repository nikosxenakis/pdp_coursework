/*
 * Description: This is the provided functionality from the actor_framework
 * Warning: Use it as it is any changes will lead to failure
*/

#ifndef ACTOR_FRAMEWORK_INTERFACE_H
#define ACTOR_FRAMEWORK_INTERFACE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

/**
 * @brief Defines of the commands that the framework uses
 */
#define COMMAND 0
#define ACTOR_TYPE 1
#define ACTOR_ID 2
#define WORKERS_NUM 3
#define WORKER_PID 4
#define INIT_ACTORS_NUM 5
#define MAX_ACTORS_NUM 6

/**
 * @brief Number of elements in a message
 */
#define MESSAGE_SIZE 17

/**
 * @class Message
 * @details class containing the array that contains the data for communication across the processes
 */
class Message {

private:
	/**
	 * @brief array containing the data of the message
	 */
	float message_data[MESSAGE_SIZE];

public:
	/**
	 * @brief declaring friend class Messager providing access to the message_data
	 */
	friend class Messenger;

	/**
	 * @brief Message constructor
	 */
	Message();

	/**
	 * @param pos Position of a specific element in the data array
	 * @return Returns the float data for the given position
	 */
	float get(int pos);

	/**
	 * @param pos Position of a specific element in the data array
	 * @return data a data number to set in the given position
	 */
	void set(int pos, float data);
};

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

	/**
	 * @brief Sends message to the master to kill all actors, after this call the simulation is finilised
	 */
	void kill_all();
};

/**
 * @class Actor_framework
 * @brief Primary interface of actor framework
 * @details A class that provides a set of functions for the simulation to initialise the framework
 */
class Actor_framework {

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
 	static void register_spawn_actor(Actor* (spawn_actor)(Message message));

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
