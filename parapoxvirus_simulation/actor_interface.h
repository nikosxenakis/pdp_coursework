#ifndef ACTOR_INTERFACE_H
#define ACTOR_INTERFACE_H

#include "actor_types.h"
#include "message.h"

class Actor_interface {

public:
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
