#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include <vector>
#include <map>
#include "actor_types.h"
#include "message.h"
#include "messenger.h"

using namespace std;

// typedef enum Actor_state {
// 	STATE_0 = 0,
// 	STATE_1,
// 	STATE_2
// } Actor_state;


class Actor
{

protected:
	int id;
	int master_pid;
	int worker_pid;
	int type;
	map<int, vector<Actor*>> known_actors;

public:
	Actor(int id, int master_pid, int worker_pid);
	int get_id();
	void print();
	int get_type();
	virtual void compute() = 0;
	void create_actor(int actor_type);
	void die();
	Actor* get_actor(int actor_id);
	void discover_actor(int worker_pid, Actor *actor);
	int find_worker(int actor_id);
	void send_msg(int actor_id, Message message);
	virtual void parse_message(Message message) = 0;
};

#endif
