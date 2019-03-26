#ifndef SIMULATION_H
#define SIMULATION_H

#include <assert.h>
#include "actor_framework_interface.h"
#include "clock.h"
#include "cell.h"
#include "squirrel.h"

#define CELLS_NO 16
#define CLOCKS_NO 1

/**
 * @brief Spawns a new actor specific for the simulation passing data to the appropriate constructor using the message data structure
 * @param the input message containing information for the new actor
 * @return Retuns a pointer to the new spawned actor
 */
Actor* spawn_actor(Message message);

/**
 * @brief parses the input argument of the simulation
 * @return Returns a message containing the input data
 */
Message parse_args(int argc, char* argv[]);

/**
 * @brief main function of the simulation
 */
int main(int argc, char* argv[]);

#endif
