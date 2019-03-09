#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include <vector>
#include <map>
#include "actor_interface.h"
#include "actor_types.h"
#include "message.h"
#include "messenger.h"

using namespace std;

#define COMPUTE 0
#define PARSE_MESSAGE 1

class Actor: public Actor_interface {

private:
	int id;
	int master_pid;
	int worker_pid;
	int state;
	map<int, vector<Actor*>> known_actors;
	map<int, void (*)(Actor*)> compute_map;
	map<int, void (*)(Actor*, Message)> parse_message_map;

protected:

	int type;

public:

	void print();
	int get_type();
	void compute();
	void parse_message(Message message);
	void discover_actor(int worker_pid, Actor *actor);
	void forget_actor(int actor_id);
	int find_worker(int actor_id);

	// Interface
	Actor(int id, int master_pid, int worker_pid);
	virtual ~Actor() = default;
	int get_id();
	void create_actor(int actor_type);
	void die();
	Actor* get_actor(int actor_id);
	void send_msg(int actor_id, Message message);
	void set_state(int state);
	void register_state(int type, int state, void (f)(Actor*));
	void register_state(int type, int state, void (f)(Actor*, Message));
};

#endif
