#include "actor.hpp"

Actor::Actor() {
	this->actor_id = 0;
}

Actor::~Actor() {
}

void Actor::spawn() {
	std::cout << "Actor spawns" << std::endl;
}

void Actor::die() {
	std::cout << "Actor dies" << std::endl;
}

std::string Actor::getInfo(){
	return "hi";
	// std::stringstream ss;
	// ss << this->actor_id;
	// return ss.str();
}
