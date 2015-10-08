# bayonet - C++ Bayesian networks library
# Copyright (C) 2015  Massimiliano Patacchiola
# Author: Massimiliano Patacchiola
# email: 


CC=g++
CFLAGS=-Wall -std=c++11 -fexceptions -I/usr/local/include/bayonet

compile:
	@echo
	@echo "===     Bayonet - C++ Bayesian network library     ==="
	@echo "===    Copyright Massimiliano Patacchiola 2015     ==="
	@echo "===                GNU v2.0 license                ==="
	@echo
	mkdir -p ./build #check if the folder exist then create it
	mkdir -p ./build/obj
	mkdir -p ./build/exec
	g++ $(CFLAGS) -Iinclude -c ./monty_hall.cpp -o ./build/obj/monty_hall.o

	@echo
	@echo "=== Compiling example files ==="
	@echo
	@echo "..... monty_hall"
	g++ -o ./build/exec/monty_hall ./build/obj/monty_hall.o -lbayonet


remove:
	@echo
	@echo "=== Removing example files ==="
	@echo
	rm ./build/obj/monty_hall.o ./build/exec/monty_hall 







