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

private:
	
protected:
	int id;
	int master_pid;
	int worker_pid;
	int type;
	int state;
	map<int, vector<Actor*>> known_actors;
	map<int, void (*)(Actor*)> compute_map;
	map<int, void (*)(Actor*, Message)> parse_message_map;

public:
	Actor(int id, int master_pid, int worker_pid);
	int get_id();
	void print();
	int get_type();
	void compute();
	void parse_message(Message message);
	void create_actor(int actor_type);
	void die();
	Actor* get_actor(int actor_id);
	void discover_actor(int worker_pid, Actor *actor);
	int find_worker(int actor_id);
	void send_msg(int actor_id, Message message);
	void set_state(int state);
	virtual ~Actor() = default;
};

#endif
