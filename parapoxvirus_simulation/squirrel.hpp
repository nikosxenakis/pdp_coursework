#ifndef SQUIRREL
#define SQUIRREL

#include <iostream>
#include <string>
#include "../actor_framework/actor.hpp"

class Squirrel: public Actor
{

private:

public:
	Squirrel();
	~Squirrel();
	void spawn();
	void die();
	virtual void act() override;
	std::string getInfo();
};

#endif

