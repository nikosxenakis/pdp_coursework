#ifndef SQUIRREL_H
#define SQUIRREL_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>
#include "actor_framework_interface.h"
#include "squirrel-functions.h"
#include "actor_types.h"
#include "simulation_message_types.h"
#include "simulation_commands.h"

#define CLOCK_ID 16

// States
#define LIVE 0
#define INTERACT 1
#define DIED 2

// Squirrels defines
#define STEPS_MEMORY 50
#define NEXT_STEP_DELAY 10

using namespace std;
using namespace std::chrono;

/**
 * @class Squirrel
 * @brief Derived class of Actor
 * @details class Squirrel is an Actor holding vital information for this entity and providing functionality
 */
class Squirrel: public Actor {

public:
	/**
	 * @brief unique negative number for each squirrel actor
	 */
	long seed;

	/**
	 * @brief float number taking values from 0.0 to 0.999 describing the x position in the landscape
	 */
	float x;

	/**
	 * @brief float number taking values from 0.0 to 0.999 describing the y position in the landscape
	 */
	float y;

	/**
	 * @brief float number taking values 1 or 0 describing if a squirrel is healthy or not if it is set to 0 it cannot change to 1
	 */
	int healthy;

	/**
	 * @brief Squirrels steps number
	 */
	int step_no;

	/**
	 * @brief Squirrels infected steps
	 */
	int infected_steps;

	/**
	 * @brief Counter controlling when Squirrel computes
	 * 1 out of DELAY=1000 times that the squirrel will be in this compute will execute the functions
	 * Squirrel does not have the consept of time it will use the counter variable to decide if it will execute its main functionality
	 */
	int counter;

	/**
	 * @brief Vector containing the population influx of the last 50 visited Cells
	 */
 	vector<int> population_influx;

 	/**
	 * @brief Vector containing the infection level of the last 50 visited Cells
	 */
	vector<int> infection_level;

 	/**
	 * @brief Time in milliseconds that the previous step was
	 */
	milliseconds begin_time;

	/**
	 * @brief Squirrel constructor
	 * @param id id of the squirrel
	 * @param workers_num number of active workers it should be passed in the parent class
	 * @param x initial position x of squirrel
	 * @param y initial position y of squirrel
	 * @param healthy initial health status of squirrel
	 */
	Squirrel(int id, int workers_num, float x, float y, int healthy);

	/**
	 * @brief Squirrel destructor
	 */
	~Squirrel();

	/**
	 * @brief 
	 * If a squirrel steps and ends up in the same cell, this counts as a squirrel arriving in the cell.
	 * Squirrels that are born in a cell do not contribute to the populationInflux at the instant that they are born
	 */
	void move();

	/**
	 * @brief After every 50 steps, a squirrel will reproduce with a probability depending only on the average populationInflux of all of the cells they have visited in their last 50 steps
	 */
	void birth();

	/**
	 * @brief After every step, the squirrel will catch the disease with a probability depending on the infectionLevel of all the cells they have visited in the last 50 steps
	 */
	void catch_disease();

	/**
	 * @brief An infected squirrel will live for a minimum of 50 steps, after this then they will die with a fixed probability of 1/6 for each step after
	 */
	void die();

	/**
	 * @brief Decide if the squirrel should act
	 */
	bool should_act();
};

#endif
