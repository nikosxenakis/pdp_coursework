#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <stdio.h>

typedef struct Input_data {
	int cells;
	int squirells;
	int infection_level;
	int max_months;
	int workers_num;
} Input_data;

Input_data* init_input_data(int argc, char* argv[]);
void print_input_data(Input_data* input_data);

#endif
