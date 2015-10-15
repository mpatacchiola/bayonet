# bayonet - C++ Bayesian networks library
# Copyright (C) 2015  Massimiliano Patacchiola
# Author: Massimiliano Patacchiola
# email: 


CC=g++
CFLAGS=-Wall -std=c++11 -O3 -fexceptions -I/usr/local/include/bayonet

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
	g++ $(CFLAGS) -Iinclude -c ./grass.cpp -o ./build/obj/grass.o
	g++ $(CFLAGS) -Iinclude -c ./earthquake.cpp -o ./build/obj/earthquake.o


	@echo
	@echo "=== Compiling example files ==="
	@echo
	@echo "..... monty_hall"
	g++ -o ./build/exec/monty_hall ./build/obj/monty_hall.o -lbayonet
	@echo
	@echo "..... grass"
	g++ -o ./build/exec/grass ./build/obj/grass.o -lbayonet
	@echo
	@echo "..... earthquake"
	g++ -o ./build/exec/earthquake ./build/obj/earthquake.o -lbayonet

remove:
	@echo
	@echo "=== Removing example files ==="
	@echo
	rm ./build/obj/monty_hall.o ./build/exec/monty_hall 
	rm ./build/obj/grass.o ./build/exec/grass
	rm ./build/obj/earthquake.o ./build/exec/earthquake





