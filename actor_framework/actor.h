#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include <vector>
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
	 * @unique identification of actor
	 */
	int id;
	int workers_num;
	int state;
	int type;
	map<int, void (*)(Actor*)> compute_map;
	map<int, void (*)(Actor*, Message)> parse_message_map;


	int get_worker(int actor_id);

protected:

	void register_state(int state, void (f)(Actor*));
	void register_state(int state, void (f)(Actor*, Message));

	void create_actor(Message message);
	void kill_actor();
	void kill_all();

public:

	Actor(int id, int type, int workers_num);
	virtual ~Actor() = 0;

	int get_id();

	void set_state(int state);

	void print();

	void compute();
	void process(Message message);
	void send_msg(int actor_id, Message message);

};

#endif
