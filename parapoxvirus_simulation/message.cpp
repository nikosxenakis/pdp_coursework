#include "message.h"

Message::Message() {
}

Message::Message(int command) {
	this->command = command;
}

Message::Message(int command, int id, int type) {
	this->command = command;
	this->actor_id = actor_id;
	this->actor_type = actor_type;
}

void Message::print() {
	cout << "Message: command = " << this->get_string_command(this->command) << ", actor_id = " << this->actor_id << ", actor_type = " << this->actor_type << endl;
}

string Message::get_string_command(int command) {
	if(command == 0)
		return "SPAWN_ACTOR_COMMAND";
	if(command == 1)
		return "KILL_ACTOR_COMMAND";
	if(command == 10)
		return "KILL_WORKER_COMMAND";
}
