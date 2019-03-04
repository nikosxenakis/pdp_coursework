//
//  main.cpp
//
//  Copyright © 2019 Nikos Xenakis. All rights reserved.
//

#include "../actor_framework/actor.hpp"
#include "squirrel.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	/* code */
		Squirrel * squirrel = new Squirrel();
		std::cout << squirrel->getInfo();
		squirrel->die();

	// std::cout << squirrel->getInfo();
	return 0;
}