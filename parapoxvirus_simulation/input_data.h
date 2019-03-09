#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

class Input_Data
{
public:
	int clocks;
	int cells;
	int squirells;
	int infection_level;
	int init_actors_num;
	int max_months;
	int max_actors_num;

	Input_Data(int argc, char* argv[]);
	~Input_Data();
	void print();

};

#endif
