#include "message.h"

Message::Message() {
	this->message_data = {
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1.0,
		-1.0
	};
}

Message::Message(Message_data message_data) {
	this->message_data = message_data;
}

void Message::print() {
	cout << "Message: command = " << this->get_string_command() << ", actor_id = " << this->message_data.actor_id << ", actor_type = " << this->message_data.actor_type \
	<< ", worker_pid = " << this->message_data.worker_pid << ", actor_id_dest = " << this->message_data.actor_id_dest << ", healthy = " << this->message_data.healthy \
	<< ", population_influx = " << this->message_data.population_influx << ", infection_level = " << this->message_data.infection_level << ", x = " << this->message_data.x << ", y = " << this->message_data.y << endl;
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
