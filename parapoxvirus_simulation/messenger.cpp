#include "messenger.h"

MPI_Datatype Messenger::Message_type;

void Messenger::init_types() {

}

void Messenger::send_message(int worker_pid, Message message) {
	MPI_Bsend(&message, 1, MPI_INT, worker_pid, 0, MPI_COMM_WORLD);
}

void Messenger::send_message(int worker_pid, Message message, int blocking) {
	MPI_Send(&message, 1, MPI_INT, worker_pid, 0, MPI_COMM_WORLD);
}

Message Messenger::receive_message(int source_pid) {
	Message message();
	MPI_Recv(&message, 1, MPI_INT, source_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	return message;
}