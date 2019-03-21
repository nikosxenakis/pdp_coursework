#include "message.h"

Message::Message() {
	for (int i = 0; i < MESSAGE_SIZE; ++i)
		this->message_data[i] = -1.0;
}

float Message::get(int pos) {
	assert(pos < MESSAGE_SIZE);
	return this->message_data[pos];
}

void Message::set(int pos, float data) {
	assert(pos < MESSAGE_SIZE);
	this->message_data[pos] = data;
}
