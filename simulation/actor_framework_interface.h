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

#define MESSAGE_SIZE 17

// framework message fields DON'T OVERRIDE THEM
// add new fields from 8 to 16
#define COMMAND 0
#define ACTOR_TYPE 1
#define ACTOR_ID 2
#define WORKERS_NUM 3
#define WORKER_PID 4
#define INIT_ACTORS_NUM 5
#define MAX_ACTORS_NUM 6

// class Message {
// public:
// 	float message_data[MESSAGE_SIZE];
// 	Message();
// 	void set(int pos, float data);
// 	float get(int pos);
// };
Message::Message();
Message::set(int pos, float data);

Message::get(int pos);

class Actor {
private:
	int id;
	int master_pid;
	int worker_pid;
	int state;
	int type;
	map<int, void (*)(Actor*)> compute_map;
	map<int, void (*)(Actor*, Message)> parse_message_map;

protected:
	void register_state(int state, void (f)(Actor*));
	void register_state(int state, void (f)(Actor*, Message));
public:
	Actor(int id, int master_pid, int worker_pid, int workers_num);
	virtual ~Actor();
	int get_type();
	int get_id();

	void set_type(int type);
	void set_state(int state);

	void create_actor(Message message);
	void kill_actor();
	void kill_all();
	void send_msg(int actor_id, Message message);
};


class Actor_framework {
public:
	static void register_init_actors(void (init_actors)(Message message));
	static void register_spawn_actor(Actor* (spawn_actor)(Message message), Message message);
	static void spawn_actor(Message message);
	static void actor_framework(Message message);
};

#endif
