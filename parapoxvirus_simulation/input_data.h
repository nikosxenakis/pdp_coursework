#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

class Input_Data
{
public:
	int cells;
	int squirells;
	int infection_level;
	int max_months;
	int workers_num;
	int max_actors_num;

	Input_Data(int argc, char* argv[]);
	~Input_Data();
	void print();

};

#endif
