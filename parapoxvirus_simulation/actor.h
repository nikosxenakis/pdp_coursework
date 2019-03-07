#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>

using namespace std;

#define ACTOR_TYPE_NONE 0
#define ACTOR_TYPE_SQUIRREL 1

// typedef enum Actor_state {
// 	STATE_0 = 0,
// 	STATE_1,
// 	STATE_2
// } Actor_state;

// typedef struct Actor {
// } Actor;

// static void check_messages();
// static void process_message();
// static void compute();
// static void create_actor();
// static void die();
class Actor
{

protected:
	int id;
	int type;

public:
	Actor(int id);
	int get_id();
	void print();
	int get_type();

};

#endif
