#ifndef SQUIRREL_H
#define SQUIRREL_H

#include "actor.h"
#include <iostream>

using namespace std;

class Squirrel: public Actor
{

public:
	Squirrel(int id, int master_pid);
	void compute() override;
};

#endif
