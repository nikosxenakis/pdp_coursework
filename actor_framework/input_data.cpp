#include "input_data.h"

Input_data::Input_data(int argc, char* argv[]) {
	this->clocks = atoi(argv[1]);
	this->cells = atoi(argv[2]);
	this->squirells = atoi(argv[3]);
	this->init_actors_num = this->cells + this->squirells + this->clocks;
	this->infection_level = atoi(argv[4]);
	this->max_months = atoi(argv[5]);
	this->max_actors_num = atoi(argv[6]);
	this->x = 0;
	this->y = 0;
}

Input_data::~Input_data() {

}

void Input_data::print() {
	cout << "CELLS: " << this->cells << endl;
	cout << "SQUIRRELS: " << this->squirells << endl;
	cout << "INFECTION_LEVEL: " << this->infection_level << endl;
	cout << "MAX_MONTHS: " << this->max_months << endl;
	cout << "MAX_ACTORS_NUM: " << this->max_actors_num << endl;
}
