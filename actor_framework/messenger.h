#ifndef MESSENGER_H
#define MESSENGER_H

#include <mpi.h>
#include <assert.h>
#include "message.h"

/**
 * @class Messenger
 * @brief class that provides functionality to send messages over MPI
 */
class Messenger {

private:
	/**
	 * @brief MPI_Datatype describing the message type
	 */
	static MPI_Datatype Message_type;

public:

	/**
	 * @brief initialises the MPI_Datatype
	 */
	static void init_types();

	/**
	 * @brief sends an ansyncronous buffered message
	 * @param worker_pid the destination process
	 * @param the message data to send
	 */
	static void send_message(int worker_pid, Message message);

	/**
	 * @brief sends an blocking message
	 * @param worker_pid the destination process
	 * @param the message data to send
	 */
	static void send_blocking_message(int worker_pid, Message message);

	/**
	 * @brief receives a message
	 * @param source_pid the source process
	 * @return Returns the received message
	 */
	static Message receive_message(int source_pid);
};

#endif
