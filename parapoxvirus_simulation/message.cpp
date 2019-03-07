#include "message.h"

Message::Message(int command, Actor *actor) {
	this->command = command;
	this->actor_id = actor->get_id();
	this->actor_type = actor->get_type();
}

void Message::print() {
	cout << "Message: " << this->command << " " << this->actor_id << " " << this->actor_type << endl;
}
