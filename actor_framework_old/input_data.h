#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

class Input_data {
public:
	int clocks;
	int cells;
	int squirells;
	int infection_level;
	int init_actors_num;
	int max_months;
	int max_actors_num;
	int healthy;
	float x;
	float y;

	Input_data(int argc, char* argv[]);
	~Input_data();
	void print();

};

#endif
