#COMPILER OPTIONS
CFLAGS := -std=c++11 # -g -Wall -O3 -cc=icc
CC := mpicxx

actor_framework: actor.cpp
	$(CC) $(CFLAGS) -g -c -o actor_framework.o $<

clean:
	@echo " Cleaning..."
	@rm -rf actor_framework.o

.PHONY: clean actor_framework
