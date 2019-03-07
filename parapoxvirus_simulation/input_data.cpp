#include "input_data.h"

Input_Data::Input_Data(int argc, char* argv[]) {
	this->cells = atoi(argv[1]);
	this->squirells = atoi(argv[2]);
	this->infection_level = atoi(argv[3]);
	this->max_months = atoi(argv[4]);
	this->workers_num = atoi(argv[5]);
}

Input_Data::~Input_Data() {

}

void Input_Data::print() {
	cout << "CELLS: " << this->cells << endl;
	cout << "SQUIRRELS: " << this->squirells << endl;
	cout << "INFECTION_LEVEL: " << this->infection_level << endl;
	cout << "MAX_MONTHS: " << this->max_months << endl;
	cout << "WORKERS_NUM: " << this->workers_num << endl;
}
