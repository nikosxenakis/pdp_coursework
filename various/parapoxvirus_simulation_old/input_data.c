#include "input_data.h"

Input_data* init_input_data(int argc, char* argv[]) {
	Input_data* input_data = malloc(sizeof(Input_data));

	input_data->cells = atoi(argv[1]);
	input_data->squirells = atoi(argv[2]);
	input_data->infection_level = atoi(argv[3]);
	input_data->max_months = atoi(argv[4]);
	input_data->workers_num = atoi(argv[5]);
	return input_data;
}

void print_input_data(Input_data* input_data) {
	printf("CELLS: %d\n", input_data->cells);
	printf("SQUIRRELS: %d\n", input_data->squirells);
	printf("INFECTION_LEVEL: %d\n", input_data->infection_level);
	printf("MAX_MONTHS: %d\n", input_data->max_months);
	printf("WORKERS_NUM: %d\n", input_data->workers_num);
}
