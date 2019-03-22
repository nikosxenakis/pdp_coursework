#ifndef MESSAGE_H
#define MESSAGE_H

#include <mpi.h>
#include <assert.h>
#include "framework_commands.h"
#include "framework_message_types.h"

/**
 * @brief Number of elements in a message
 */
#define MESSAGE_SIZE 17

/**
 * @class Message
 * @details class containing the array that contains the data for communication across the processes
 */
class Message {

private:
	/**
	 * @brief array containing the data of the message
	 */
	float message_data[MESSAGE_SIZE];

public:
	/**
	 * @brief declaring friend class Messager providing access to the message_data
	 */
	friend class Messenger;

	/**
	 * @brief Message constructor
	 */
	Message();

	/**
	 * @param pos Position of a specific element in the data array
	 * @return Returns the float data for the given position
	 */
	float get(int pos);

	/**
	 * @param pos Position of a specific element in the data array
	 * @return data a data number to set in the given position
	 */
	void set(int pos, float data);
};

#endif
