#include "message.h"

Message::Message() {
	for (int i = 0; i < MESSAGE_SIZE; ++i)
		this->message_data[i] = -1.0;
}

// Message::Message(float message_data[MESSAGE_SIZE]) {
// 	for (int i = 0; i < MESSAGE_SIZE; ++i)
// 		this->message_data[i] = message_data[i];
// }

void Message::print() {
	cout << "Message: ";

	for (int i = 0; i < MESSAGE_SIZE; ++i)
	{
		cout << "[" << this->get_field_name(i) << "]=" << this->message_data[i] << " ";
	}
	cout << endl;

}

string Message::get_field_name(int pos) {
	switch(pos) {
		// case 0:
		// 	return "COMMAND";
		// case 1:
		// 	return "ACTOR_TYPE";
		// case 2:
		// 	return "ACTOR_ID";
		// case 3:
		// 	return "ACTOR_ID_DEST";
		// case 4:
		// 	return "POPULATION_INFLUX";
		// case 5:
		// 	return "INFECTION_LEVEL";
		// case 6:
		// 	return "HEALTHY";
		// case 7:
		// 	return "WORKER_PID";
		// case 8:
		// 	return "X";
		// case 9:
		// 	return "Y";
		// case 10:
		// 	return "MAX_MONTHS";
		// case 11:
		// 	return "CLOCKS";
		// case 12:
		// 	return "CELLS";
		// case 13:
		// 	return "SQUIRRELS";
		// case 14:
		// 	return "INIT_ACTORS_NUM";
		// case 15:
		// 	return "MAX_ACTORS_NUM";
		// case 16:
		// 	return "WORKERS_NUM";
		default:
			return "UNREGISTERED";
	}
}

string Message::get_string_command() {
	return "dsa";
	// if(this->message_data.command == 0)
	// 	return "SPAWN_ACTOR_COMMAND";
	// else if(this->message_data.command == 1)
	// 	return "KILL_ACTOR_COMMAND";
	// else if(this->message_data.command == 2)
	// 	return "DISCOVER_ACTOR_COMMAND";
	// else if(this->message_data.command == 3)
	// 	return "FORGET_ACTOR_COMMAND";
	// else if(this->message_data.command == 10)
	// 	return "KILL_WORKER_COMMAND";
	// else if(this->message_data.command == 11)
	// 	return "START_WORKER_COMMAND";
	// else if(this->message_data.command == 20)
	// 	return "VISIT_ACTOR_COMMAND";
	// else if(this->message_data.command == 21)
	// 	return "PING_ACTOR_COMMAND";
	// else if(this->message_data.command == 30)
	// 	return "TIMESTEP_START";
	// else if(this->message_data.command == 31)
	// 	return "TIMESTEP_END";
	// else if(this->message_data.command == 32)
	// 	return "TIMESTEP_END_GAVE_BIRTH";
}

void Message::set(int pos, float data) {
	this->message_data[pos] = data;
}

float Message::get(int pos) {
	return this->message_data[pos];
}
