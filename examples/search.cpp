/* 
 * bayonet - C++ Bayesian networks library
 * Copyright (C) 2015  Massimiliano Patacchiola
 * Author: Massimiliano Patacchiola
 * email:  
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
*/

/*
 * In this example a Multi-Connected network is used for
 * showing the Search algorithms. It is possible to modify
 * the architecture for seeing the effect on sorting
 * and searching. 
 * An image representing the network is present in the
 * folder, look for search.png
 *
*/

#include <iostream>
#include<bayonet/Bayesnet.h>
#include<bayonet/Bayesnode.h>


int main()
{

 enum node{
  A=0,
  B=1,
  C=2,
  D=3,
  E=4,
  F=5,
  G=6,
  H=7,
  I=8
 };

 //The network has 9 boolen nodes
 bayonet::Bayesnet myNet({2, 2, 2, 2, 2, 2, 2, 2, 2});

 //Creating the network structure
 myNet.AddEdge(A,B);
 myNet.AddEdge(A,C);
 myNet.AddEdge(A,D);

 myNet.AddEdge(B,E);

 //Removing this two edges the network
 //becomes single connected
 myNet.AddEdge(C,E); // <--- Remove if you want single connected network
 myNet.AddEdge(C,F); // <--- Remove if you want single connected network

 myNet.AddEdge(D,F);

 myNet.AddEdge(E,H);

 myNet.AddEdge(F,G);
 myNet.AddEdge(H,I);

 //Some generic info
 std::cout << "TOTAL NODES: " <<  myNet.ReturnNumberOfNodes() << std::endl;
 std::cout << "TOTAL EDGES: " << myNet.ReturnNumberOfEdges() << std::endl;

 //Checking if multi-connected
 bool is_multi_connected = myNet.IsMultiConnected();
 std::cout << "IS MULTI-CONNECTED: ";
 if(is_multi_connected==true) std::cout << "TRUE" << std::endl;
 if(is_multi_connected==false) std::cout << "FALSE" << std::endl;
 
 //Checking if tree
 bool is_tree = myNet.IsTree();
 std::cout << "IS TREE: ";
 if(is_tree==true) std::cout << "TRUE" << std::endl;
 if(is_tree==false) std::cout << "FALSE" << std::endl;

 //Showing the topological order.
 //The topological order is a particular order of the nodes
 //that permit to dispose the nodes from the root to the leafs.
 //for every directed edge X --> Y from vertex X to vertex Y, 
 //X comes before Y in the topological sorting.
 std::list<unsigned int> topo_list = myNet.ReturnTopologicalList();
 std::cout << "TOPOLOGICAL ORDER: ";
 for(auto it_topo=topo_list.begin(); it_topo!=topo_list.end(); ++it_topo){
  std::cout << *it_topo << " ";
 }
 std::cout << std::endl;

 //Deep First Search (DFS) is an algorithm for traversing a graph
 //Selecting a starting node the algorithm search as far as possible
 //along the branch.
 auto deep_list = myNet.DepthFirstSearch(A);
 std::cout << "DEEP FIRST SEARCH: ";
 for(auto it_deep=deep_list.begin(); it_deep!=deep_list.end(); ++it_deep){
  std::cout << *it_deep << " ";
 }
 std::cout << std::endl;

 //Breadth First Search (BFS) is an algorithm for traversing graph.
 //The BFS explores all the neighbours before moving to the children.
 auto breadth_list = myNet.BreadthFirstSearch(A);
 std::cout << "BREADTH FIRST SEARCH: ";
 for(auto it_breadth=breadth_list.begin(); it_breadth!=breadth_list.end(); ++it_breadth){
  std::cout << *it_breadth << " ";
 }
 std::cout << std::endl;




 return 0;
}
