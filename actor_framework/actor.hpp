#ifndef ACTOR
#define ACTOR

#include <iostream>
#include <string>
#include <sstream>

class Actor
{

private:
	int actor_id;

public:
	Actor();
	~Actor();
	void spawn();
	void die();
	virtual void act() = 0;
	std::string getInfo();
	// void sendMsg();
	// void recvMsg();
};

#endif
