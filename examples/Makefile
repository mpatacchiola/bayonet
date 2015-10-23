# bayonet - C++ Bayesian networks library
# Copyright (C) 2015  Massimiliano Patacchiola
# Author: Massimiliano Patacchiola
# email: 


CC=g++
CFLAGS=-Wall -std=c++11 -O3 -fexceptions -I/usr/local/include/bayonet

#to compress the files for the handwritten digit example: tar -jcvf handwritten_dataset.tar pendigits.tes pendigits.tra
#to uncompress the files: -jxvf handwritten_dataset.tar -C ./build/exec/

compile:
	@echo
	@echo "===     Bayonet - C++ Bayesian network library     ==="
	@echo "===    Copyright Massimiliano Patacchiola 2015     ==="
	@echo "===                GNU v2.0 license                ==="
	@echo
	mkdir -p ./build #check if the folder exist then create it
	mkdir -p ./build/obj
	mkdir -p ./build/exec


	@echo
	@echo "=== Compiling example files ==="
	@echo
	@echo "..... monty_hall"
	g++ $(CFLAGS) -Iinclude -c ./monty_hall.cpp -o ./build/obj/monty_hall.o
	g++ -o ./build/exec/monty_hall ./build/obj/monty_hall.o -lbayonet
	@echo
	@echo "..... grass"
	g++ $(CFLAGS) -Iinclude -c ./grass.cpp -o ./build/obj/grass.o
	g++ -o ./build/exec/grass ./build/obj/grass.o -lbayonet
	@echo
	@echo "..... earthquake"
	g++ $(CFLAGS) -Iinclude -c ./earthquake.cpp -o ./build/obj/earthquake.o
	g++ -o ./build/exec/earthquake ./build/obj/earthquake.o -lbayonet
	@echo
	@echo "..... search"
	g++ $(CFLAGS) -Iinclude -c ./search.cpp -o ./build/obj/search.o
	g++ -o ./build/exec/search ./build/obj/search.o -lbayonet
	@echo
	@echo "..... naive bayes handwritten digits"
	g++ $(CFLAGS) -Iinclude -c ./naive_bayes_handwritten_digits.cpp -o ./build/obj/naive_bayes_handwritten_digits.o
	g++ -o ./build/exec/naive_bayes_handwritten_digits ./build/obj/naive_bayes_handwritten_digits.o -lbayonet

	@echo
	@echo "=== Uncompressing the dataset ==="
	@echo
	tar -jxvf handwritten_dataset.tar -C ./build/exec/


remove:
	@echo
	@echo "=== Removing example files ==="
	@echo
	rm ./build/obj/monty_hall.o ./build/exec/monty_hall 
	rm ./build/obj/grass.o ./build/exec/grass
	rm ./build/obj/earthquake.o ./build/exec/earthquake
	rm ./build/obj/search.o ./build/exec/search
	rm ./build/obj/naive_bayes_handwritten_digits.o ./build/exec/naive_bayes_handwritten_digits



