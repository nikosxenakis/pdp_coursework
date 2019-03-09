#include "message.h"

Message::Message() {
}

Message::Message(int command) {
	this->command = command;
}

Message::Message(int command, int actor_type) {
	this->command = command;
	this->actor_type = actor_type;
}

Message::Message(int command, int actor_id, int data2) {
	this->command = command;
	this->actor_id = actor_id;
	if(command == VISIT_ACTOR_COMMAND) {
		this->actor_id_dest = data2;
	}
	else {
		this->actor_type = data2;
	}
}

Message::Message(int command, int actor_id, int actor_type, int worker_pid) {
	this->command = command;
	this->actor_id = actor_id;
	this->actor_type = actor_type;
	this->worker_pid = worker_pid;
}

void Message::print() {
	cout << "Message: command = " << this->get_string_command() << ", actor_id = " << this->actor_id << ", actor_type = " << this->actor_type \
	<< ", worker_pid = " << this->worker_pid << endl;
}

string Message::get_string_command() {
	if(this->command == 0)
		return "SPAWN_ACTOR_COMMAND";
	else if(this->command == 1)
		return "KILL_ACTOR_COMMAND";
	else if(this->command == 2)
		return "DISCOVER_ACTOR_COMMAND";
	else if(this->command == 3)
		return "FORGET_ACTOR_COMMAND";
	else if(this->command == 10)
		return "KILL_WORKER_COMMAND";
	else if(this->command == 11)
		return "START_WORKER_COMMAND";
	else if(this->command == 20)
		return "VISIT_ACTOR_COMMAND";
	else if(this->command == 21)
		return "PING_ACTOR_COMMAND";
}
