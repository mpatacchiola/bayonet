# bayonet - C++ Bayesian networks library
# Copyright (C) 2015  Massimiliano Patacchiola
# Author: Massimiliano Patacchiola
# email: 


CC=g++
CFLAGS=-Wall -std=c++11 -fPIC

compile:
	@echo
	@echo "===     Bayonet - C++ Bayesian network library     ==="
	@echo "===    Copyright Massimiliano Patacchiola 2015     ==="
	@echo "===                GNU v2.0 license                ==="
	@echo
	mkdir -p ./bin #check if the folder exist then create it
	mkdir -p ./bin/obj
	mkdir -p ./bin/lib
	g++ $(CFLAGS) -Iinclude -c ./src/Bayesnode.cpp -o ./bin/obj/Bayesnode.o
	g++ $(CFLAGS) -Iinclude -c ./src/Bayesnet.cpp -o ./bin/obj/Bayesnet.o

	@echo
	@echo "=== Starting creation of Shared Library ==="
	g++ -fPIC -shared -Wl,-soname,libbayonet.so.1 -o ./bin/lib/libbayonet.so.1.0 ./bin/obj/Bayesnode.o ./bin/obj/Bayesnet.o

	@echo
	@echo "=== Creating the Static Library ==="
	ar rcs ./bin/lib/libbayonet.a ./bin/obj/Bayesnode.o ./bin/obj/Bayesnet.o

install:

	@echo
	@echo "=== Copying file in the system folders ==="
	mkdir -p /usr/local/include/bayonet
	cp ./include/* /usr/local/include/bayonet
	cp ./bin/lib/libbayonet.so.1.0 /usr/local/lib

	@echo
	@echo "=== Adding Symbolic link to shared library ==="
	ln -sf /usr/local/lib/libbayonet.so.1.0 /usr/local/lib/libbayonet.so
	ln -sf /usr/local/lib/libbayonet.so.1.0 /usr/local/lib/libbayonet.so.1

clean:
	rm ./bin/obj/Bayesnode.o ./bin/obj/Bayesnet.o
	rm ./bin/lib/libbayonet.so.1.0  
	rm /usr/local/lib/libbayonet.so /usr/local/lib/libbayonet.so.1
	rm /usr/local/lib/libbayonet.a ./bin/lib/libbayonet.a
	rm -r /usr/local/include/bayonet






