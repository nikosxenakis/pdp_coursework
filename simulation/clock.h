#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>
#include <assert.h>
#include "actor_framework_interface.h"
#include "actor_types.h"
#include "simulation_message_types.h"
#include "simulation_commands.h"

using namespace std;
using namespace std::chrono;

// States
#define IN_MONTH 0
#define END_OF_MONTH 1
#define FINISH 3

// Clock defines
#define CELL_NUM 16
#define TIMESTEP_DURATION 1000
#define MAX_SQUIRRELS_NO 200

class Clock: public Actor {

public:
	/**
	 * @brief Vector containing the population influx of the cells for the current month
	 */
	vector<int> population_influx;

 	/**
	 * @brief Vector containing the infection level of the cells for the current month
	 */
	vector<int> infection_level;

 	/**
	 * @brief Time in milliseconds that the month began
	 */
	milliseconds begin_time;

 	/**
	 * @brief Maximum number of simulation months
	 */
	int max_months;

 	/**
	 * @brief Current month
	 */
	int month;

 	/**
	 * @brief number of cells responded for end of month report
	 */
	int cells_ready;

 	/**
	 * @brief stringstream for the population_influx output file
	 */
	static stringstream population_influx_stream;

 	/**
	 * @brief stringstream for the infection_level_stream output file
	 */
	static stringstream infection_level_stream;

	static stringstream alive_squirrels_stream;
	static stringstream infected_squirrels_stream;

 	/**
	 * @brief number of alive squirrels
	 */
	int alive_squirrels;

 	/**
	 * @brief number of infected squirrels
	 */
	int infected_squirrels;

 	/**
	 * @brief updates the output stringstream
	 */
	void write_output_stream();

 	/**
	 * @brief writes the output files with the content of the stringstreams
	 */
	void write_output_files();

	/**
	 * @brief Clock constructor
	 * @param id id of the clock
	 * @param workers_num number of active workers it should be passed in the parent class
	 * @param max_months Number of months to simulate
	 * @param init_squirrels_no Number of initially alive squirrels
	 * @param init_inf_squirrels_no Number of initially infected squirrels
	 */
	Clock(int id, int workers_num, int max_months, int init_squirrels_no, int init_inf_squirrels_no);

	/**
	 * @brief Clock destructor
	 */
	~Clock();

};

#endif
