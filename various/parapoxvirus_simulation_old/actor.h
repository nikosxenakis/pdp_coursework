#ifndef ACTOR_H
#define ACTOR_H

typedef enum Actor_state {
	STATE_0 = 0,
	STATE_1,
	STATE_2
} Actor_state;

typedef struct Actor {
} Actor;

static void check_messages();
static void process_message();
static void compute();
static void create_actor();
static void die();

#endif

