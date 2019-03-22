#ifndef CELL_H
#define CELL_H

#include <vector>
#include <algorithm>
#include <assert.h>
#include <numeric>
#include "actor_framework_interface.h"
#include "actor_types.h"
#include "simulation_message_types.h"
#include "simulation_commands.h"

#define SIMULATE 0
#define CLOCK_ID 16

using namespace std;

class Cell: public Actor {

public:

	/**
	 * @brief current timestep in the cell
	 */
	int timestep;

	/**
	 * @brief populationInflux is a vector containing the total number of squirrels that have stepped into the specific cell at some point during the past 3 months
	 */
	vector<int> population_influx;

	/**
	 * @brief infection_level is a vector containing the total number of infected squirrels that have stepped into the specific cell at some point during the past 2 months
	 */
	vector<int> infection_level;

	/**
	 * @brief Cell constructor
	 * @param id id of the cell
	 * @param workers_num number of active workers it should be passed in the parent class
	 */
	Cell(int id, int workers_num);

	/**
	 * @brief Cell destructor
	 */
	~Cell();

	/**
	 * @brief Cell function when is visited by a squirrel
	 * @param message input message containing health status from the squirrel
	 */
	void visited(Message message);

};

#endif
