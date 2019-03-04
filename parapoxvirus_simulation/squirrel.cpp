#include "squirrel.hpp"

Squirrel::Squirrel() {
}

Squirrel::~Squirrel() {
}

void Squirrel::spawn() {
	std::cout << "Squirrel spawns" << std::endl;
}

void Squirrel::die() {
	std::cout << "Squirrel dies" << std::endl;
	this->~Squirrel();
}

void Squirrel::act() {

}

std::string Squirrel::getInfo() {
	return Actor::getInfo();
}
