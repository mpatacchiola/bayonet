Bayonet
==============

A C++ library for Bayesian networks
-----------------------------------
Bayesian networks are probabilistic graphical models, a set of random variables (called nodes) connected through directed edges. Each edge of the network represents a causal relation between two nodes. 

Bayonet is a C++ library that permits to create discrete Bayesian networks, the library has a lot of properties that we can summarize here:

- Safe memory management through smart pointers and STL containers (C++11)
- No external library required, Bayonet is self-contained
- Completely open source (GNU v2.0)
- Easy to create and manage densely connected networks
- Deterministic inference in polytree using Pearl's message passing algorithm
- Approximate inference in single and multi connected network using Gibbs sampling
- Learning the network parameters

Bayonet manages Bayesian networks as sparse graphs, indeed they do not have self-connections and cycles and the resulting number of edges is not high. To store nodes and edges an adjacency-list representation is used. Using an adjacency-matrix the amount of space necessary to store the nodes (N) is O(N^2), instead using the adjacency-list representation it is only O(N + E) where E is the number of edges.

Each node is stored as a shared pointer and the edges are stored inside the node as weak pointers. Using smart pointers it is possible to allocate and delete memory in a safe way. 


Prerequisites
--------------

To install the library you must have *make* and *g++* already installed on your system.
You can install them from a Unix system running the following commands from the terminal:

 `sudo apt-get install build-essential g++`


Installation
--------------

If you are using a Unix system you can install the library very easily:

1. Download the zip package clicking [here](https://github.com/mpatacchiola/bayonet/archive/master.zip)
2. Extract the files from the archive and save them in a folder on your computer
3. Open the terminal and browse inside that folder
4. Write `make compile` on your terminal and press Enter
5. If everything is right you will find the shared and static libraries (libbayonet.so, libbayonet.a) inside the folder *bin/lib*
6. Write `sudo make install` and press Enter
7. The installation is complete, the libraries were copied inside the system folder and they are ready to be used
8. To remove the library you have to write `make remove` in the terminal, it will delete all the files produced during the installation


Using the library
--------------

After the installation bayonet was copied on your system, inside the folder */usr/local/lib* you can see the shared library libbayonet.so and the static library libbayonet.a. Another important path is the one containing the header files, they are located at */usr/local/include/bayonet*. 
To use the shared library it is necessary to link it to your project. In g++ this is very easy, here is an example:

`g++ -Wall -std=c++11 -fPIC -I/usr/local/include/bayonet -L/usr/local/lib -Wl,--no-as-needed mycode.cpp -o mycode -lbayonet`

This command will compile the imaginary file mycode.cpp and will produce an executable file called mycode in your project directory.
Using a similar command it is also possible to use the static version of the library:

`g++ -std=c++11 -I/usr/local/lib -static -lbayonet -c /home/username/mycode.cpp`

In this case the library will be statically included inside your code.
To integrate bayonet in a different environment (ex Eclipse, Code::Blocks, etc) follow the istructions given by the producer on how to integrate an external shared library or a static one.

