#include "messenger.h"

void Messenger::send_message(int worker_pid, Message message) {
	message.print();
	// MPI_Send(&message, 0, MPI_INT, worker_pid, 0, MPI_COMM_WORLD);
}

// Message read_message(Message message) {
// 	Message message;
// 	return message;
// }
