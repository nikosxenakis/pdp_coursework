#include "message.h"

Message::Message() {
	this->message_data = {
		-1,
		-1,
		-1,
		-1,
		-1,
		-1
	};
}

// Message::Message(int command) {
// 	this->command = command;
// 	this->message_data = {
// 		-1,
// 		-1,
// 		-1,
// 		-1,
// 		-1
// 	};

// 	int actor_id;
// 	int actor_id_dest;
// 	int actor_type;
// 	int worker_pid;
// 	int healthy;
// }

Message::Message(Message_data message_data) {
	this->message_data = message_data;
}

// Message::Message(int command, int actor_type) {
// 	this->command = command;
// 	this->actor_type = actor_type;
// }

// Message::Message(int command, int actor_id, int data2) {
// 	this->command = command;
// 	this->actor_id = actor_id;
// 	this->actor_type = data2;
// }

// Message::Message(int command, int actor_id, int actor_type, int worker_pid) {
// 	this->command = command;
// 	this->actor_id = actor_id;
// 	this->actor_type = actor_type;
// 	this->worker_pid = worker_pid;

// 	if(command == VISIT_ACTOR_COMMAND) {
// 		this->actor_id_dest = actor_type;
// 		this->healthy = worker_pid;
// 	}
// }

void Message::print() {
	cout << "Message: command = " << this->get_string_command() << ", actor_id = " << this->message_data.actor_id << ", actor_type = " << this->message_data.actor_type \
	<< ", worker_pid = " << this->message_data.worker_pid << ", actor_id_dest = " << this->message_data.actor_id_dest << ", healthy = " << this->message_data.healthy << endl;
}

string Message::get_string_command() {
	if(this->message_data.command == 0)
		return "SPAWN_ACTOR_COMMAND";
	else if(this->message_data.command == 1)
		return "KILL_ACTOR_COMMAND";
	else if(this->message_data.command == 2)
		return "DISCOVER_ACTOR_COMMAND";
	else if(this->message_data.command == 3)
		return "FORGET_ACTOR_COMMAND";
	else if(this->message_data.command == 10)
		return "KILL_WORKER_COMMAND";
	else if(this->message_data.command == 11)
		return "START_WORKER_COMMAND";
	else if(this->message_data.command == 20)
		return "VISIT_ACTOR_COMMAND";
	else if(this->message_data.command == 21)
		return "PING_ACTOR_COMMAND";
	else if(this->message_data.command == 30)
		return "TIMESTEP_START";
	else if(this->message_data.command == 31)
		return "TIMESTEP_END";
	else if(this->message_data.command == 32)
		return "TIMESTEP_END_GAVE_BIRTH";
}
