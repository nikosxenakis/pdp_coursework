#include "messenger.h"

MPI_Datatype Messenger::Message_type;

void Messenger::init_types() {
    MPI_Type_vector(1, MESSAGE_SIZE, 0, MPI_FLOAT, &Messenger::Message_type);
    MPI_Type_commit(&Messenger::Message_type);
}

void Messenger::send_message(int worker_pid, Message message) {
	assert(worker_pid >= 0);
	MPI_Bsend(&message.message_data, 1, Messenger::Message_type, worker_pid, 0, MPI_COMM_WORLD);
}

void Messenger::send_blocking_message(int worker_pid, Message message) {
	assert(worker_pid >= 0);
    MPI_Send(&message.message_data, 1, Messenger::Message_type, worker_pid, 0, MPI_COMM_WORLD);
}

Message Messenger::receive_message(int source_pid) {
	Message message = Message();
	MPI_Recv(&message.message_data, 1, Messenger::Message_type, source_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	return message;
}
