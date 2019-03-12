#include "messenger.h"

#define MESSAGE_SIZE 12

MPI_Datatype Messenger::Message_type;

void Messenger::init_types() {

    MPI_Datatype type[MESSAGE_SIZE] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_FLOAT, MPI_FLOAT };
    int blocklen[MESSAGE_SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    MPI_Aint disp[MESSAGE_SIZE];

    for (int i = 0; i < MESSAGE_SIZE-2; ++i) {
        disp[i] = i*sizeof(int);
    }
    for (int i = MESSAGE_SIZE-2; i < MESSAGE_SIZE; ++i) {
        disp[i] = i*sizeof(float);
    }

    MPI_Type_create_struct(MESSAGE_SIZE, blocklen, disp, type, &Messenger::Message_type);
    MPI_Type_commit(&Messenger::Message_type);
}

void Messenger::send_message(int worker_pid, Message message) {
	MPI_Bsend(&message, 1, Messenger::Message_type, worker_pid, 0, MPI_COMM_WORLD);
}

void Messenger::send_blocking_message(int worker_pid, Message message) {
    MPI_Send(&message, 1, Messenger::Message_type, worker_pid, 0, MPI_COMM_WORLD);
}

Message Messenger::receive_message(int source_pid) {
	Message message = Message();
	MPI_Recv(&message, 1, Messenger::Message_type, source_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	return message;
}
