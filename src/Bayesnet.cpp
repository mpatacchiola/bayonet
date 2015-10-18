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

#include"Bayesnet.h"
#include<iostream>

namespace bayonet{

/**
* It create a net with a certain number of nodes and number of states.
*
* @param numberOfNodes the number of nodes to add to the network
* @param numberOfStates the number of state to assign to each node
**/
Bayesnet::Bayesnet(std::vector<unsigned int> nodesTotStatesVector) {
 //nodesVector.reserve(nodesTotStatesVector.size());
 //for(auto it=nodesTotStatesVector.begin(); it!=nodesTotStatesVector.end(); ++it){
  //auto sp = std::make_shared<Bayesnode>(*it);
  //nodesVector.push_back(sp); //filling the nodes vector
 //}
 nodesVector.reserve(nodesTotStatesVector.size());
 for(auto it=nodesTotStatesVector.begin(); it!=nodesTotStatesVector.end(); ++it){
  Bayesnode my_node(*it);
  nodesVector.push_back(my_node); //filling the nodes vector
 }
}

/**
* It destroys the object.
*
**/
Bayesnet::~Bayesnet(){}

/**
* Operator overload [] it is used to return thereference to the node stored inside the net
* It is possible to access the methods of the single node in a easier way.
* Example: net[2].IsRoot();  // It checks if the third node is a root node
* @param index the number of the element stored inside the net
* @return it returns a reference to the node
**/
Bayesnode& Bayesnet::operator[](unsigned int index){
 if (index >= nodesVector.size()) throw std::domain_error("Error: Out of Range index.");
 return nodesVector[index];
}

/**
* It adds an Edge between two nodes.
*
* @param firstNode the parent node
* @param secondNode the child node
**/
bool Bayesnet::AddEdge(unsigned int firstNode, unsigned int secondNode){
 if(firstNode == secondNode) return false;
 unsigned int first_tot_states = nodesVector[firstNode].ReturnNumberOfStates();
 nodesVector[firstNode].AddToAdjacencyList(secondNode);
 nodesVector[secondNode].conditionalTable.AddVariable(first_tot_states);
 return true;
}

/**
* It removes an Edge between two nodes.
*
* @param firstNode the parent node
* @param secondNode the child node
**/
bool Bayesnet::RemoveEdge(unsigned int firstNode, unsigned int secondNode){
 if(firstNode == secondNode) return false;
 nodesVector[firstNode].RemoveFromAdjacencyList(secondNode);
 return true;
}

/**
* It checks if an Edge between two nodes exist.
*
* @param firstNode the parent node
* @param secondNode the child node
**/
bool Bayesnet::HasEdge(unsigned int FirstNode, unsigned int SecondNode){
 return nodesVector[FirstNode].IsInAdjacencyList(SecondNode);
}

/**
* It returns the number of nodes.
*
**/
unsigned int Bayesnet::ReturnNumberOfNodes(){
 return nodesVector.size();
}

/**
* It returns the number of edges.
*
**/
unsigned int Bayesnet::ReturnNumberOfEdges(){
 unsigned int tot_edges = 0;
 for(auto it=nodesVector.begin(); it!=nodesVector.end(); ++it){
  tot_edges += it->SizeOfAdjacencyList();
 }

 return tot_edges;
}

/**
* It returns output index list
*
**/
std::list<unsigned int> Bayesnet::ReturnOutEdges(unsigned int index){
 std::list<unsigned int> temp_list;
 temp_list = nodesVector[index].ReturnAdjacencyList();
 return temp_list;
}

/**
* It returns the input index list.
*
**/
std::list<unsigned int> Bayesnet::ReturnInEdges(unsigned int index){
 std::list<unsigned int> temp_list;
 unsigned int counter = 0;
 for(auto it=nodesVector.begin(); it!=nodesVector.end(); ++it){
  if((*it).IsInAdjacencyList(index) == true) temp_list.push_back(counter);
  counter++;
 }
 return temp_list;
}

/**
* It returns the number of Incoming edges from the node specified in the index.
*
**/
unsigned int Bayesnet::ReturnNumberOutEdges(unsigned int index){
 return ReturnOutEdges(index).size();
}

/**
* It returns the number of ingoing edges from the node specified in the index.
*
**/
unsigned int Bayesnet::ReturnNumberInEdges(unsigned int index){
 return ReturnInEdges(index).size();
}


/**
* The topological sort algorithm creates a linear ordering of the vertices. If edge (u,v) appears in the graph, then u comes before v in the ordering. 
* The topological order is used during the sampling phase for sending queries to a node only when all the parents were already sorted.
* The algorithm for topological ordering use the DepthFirstSearch function to calculate the finish time of every node.
* Each node in a DAG goes from a node of higher finish time to a node of lower node finish time. 
* The problem with cyclic graph is due to the fact that back edges go from nodes of lower finish time to nodes of higher finish time 
*
* @return It returns a list of index to nodes sorted in topological order.
**/
std::list<unsigned int> Bayesnet::ReturnTopologicalList() {

 std::list<unsigned int> list_to_return;
 std::multimap<unsigned int, unsigned int> index_time_map;

 //Iterating through each node and applying the DFS algorithm to calculate the finish time
 for(unsigned int nodes_counter = 0; nodes_counter < nodesVector.size(); nodes_counter++){
  std::shared_ptr<std::list<unsigned int>> spToList;
  auto deep_list = std::make_shared<std::list<unsigned int>>();
  DepthFirstSearch(nodes_counter, deep_list);
  index_time_map.insert(std::pair<unsigned int,unsigned int>(deep_list->size(), nodes_counter));
 }

 //filling the output list from the multimap
 for(auto it_map=index_time_map.begin(); it_map!=index_time_map.end(); ++it_map){
  list_to_return.push_front(it_map->second);
 }

 return list_to_return;
}

/**
* I return a list of integers, containing the index of
* all the Root nodes.
*
**/
std::list<unsigned int> Bayesnet::ReturnRootList(){
 std::list<unsigned int> list_to_return;
 for(unsigned int i=0; i<nodesVector.size(); i++){
  if(IsRoot(i) == true) list_to_return.push_back(i);
 }
 return list_to_return;
}

/**
* I return a list of integers, containing the index of
* all the Leaf nodes.
*
**/
std::list<unsigned int> Bayesnet::ReturnLeafList(){
 std::list<unsigned int> list_to_return;
 for(unsigned int i=0; i<nodesVector.size(); i++){
  if(IsLeaf(i) == true) list_to_return.push_back(i);
 }
 return list_to_return;
}

/**
* Given the the index of a variable and the states of all
* the variables, it returns the associated probability.
*
* @param variableState
* @param parentsStates
**/
double Bayesnet::GetNodeProbability(unsigned int index, std::vector<unsigned int> variablesStatesVector){

 auto in_list = ReturnInEdges(index);
 std::vector<unsigned int> key_vector;

 for(auto it_list=in_list.begin(); it_list!=in_list.end(); ++it_list){
  key_vector.push_back(variablesStatesVector[*it_list]);
 }

 unsigned int variable_state = variablesStatesVector[index];
 double result = nodesVector[index].conditionalTable.GetProbability(variable_state, key_vector);
 return result;
}

/**
* Reset all the nodes colours to white.
* 
**/
void Bayesnet::ResetAllColours(){
 for(auto it_node=nodesVector.begin(); it_node!=nodesVector.end(); ++it_node){
  it_node->SetColour(Bayesnode::colour::WHITE);
 }
}

/**
* It returns a list with the total number of states for each node.
* 
* @return it returns the list of total states
**/
std::vector<unsigned int> Bayesnet::ReturnTotalStates(){

 std::vector<unsigned int> vector_to_return;
 for(auto it_node=nodesVector.begin(); it_node!=nodesVector.end(); ++it_node){
  vector_to_return.push_back( it_node->ReturnNumberOfStates() );
 }
 return vector_to_return;
}

/**
* It returns a list containing the index to the nodes which are not Evidence nodes.
* 
* @return it returns the list of not evidence node
**/
std::vector<unsigned int> Bayesnet::ReturnNotEvidenceNodes(){
 std::vector<unsigned int> list_to_return;
 unsigned int counter = 0;
 for(auto it_node=nodesVector.begin(); it_node!=nodesVector.end(); ++it_node){
  if(it_node->IsEvidence() == false) list_to_return.push_back( counter );
  counter++;
 }
 return list_to_return;
}

/**
* It returns a list containing the index to the nodes which are Evidence nodes.
* 
* @return it returns the list of evidence node
**/
std::vector<unsigned int> Bayesnet::ReturnEvidenceNodes(){
 std::vector<unsigned int> list_to_return;
 unsigned int counter = 0;
 for(auto it_node=nodesVector.begin(); it_node!=nodesVector.end(); ++it_node){
  if(it_node->IsEvidence() == true) list_to_return.push_back( counter );
  counter++;
 }
 return list_to_return;
}

/**
* A root node is a node without parents but with children.
* 
* @return it returns true if the node is a root node.
**/
bool Bayesnet::IsRoot(unsigned int index){
 auto out_list = ReturnOutEdges(index);
 auto in_list = ReturnInEdges(index);
 if(in_list.size() == 0 && out_list.size() > 0) return true;
 else return false;
}

/**
* A leaf node is a node without children but with parents.
* 
* @return it returns true if the node is a leaf node.
**/
bool Bayesnet::IsLeaf(unsigned int index){
 auto out_list = ReturnOutEdges(index);
 auto in_list = ReturnInEdges(index);
 if(in_list.size() > 0 && out_list.size() == 0) return true;
 else return false;
}

/**
* It return a const reference to the internal vector used to store the nodes.
* 
* @return it returns a const reference
**/
const std::vector<Bayesnode>& Bayesnet::ReturnNodesVector(){
 return nodesVector;
}


/**
* Breadth First Search algorithm. Starting from a node it performs a breadth-first traversal of the network.
*
* @param startingNode the index of the node 
* @return it returns a list of index representing the order of nodes visited
**/
std::list<unsigned int> Bayesnet::BreadthFirstSearch(unsigned int startingNode){

  std::list<unsigned int> list_to_return;

 //Mark all nodes as white (not visited)
  ResetAllColours();

 // Create a queue for BFS
 std::list<int> queue;

 // Mark the current node as visited and enqueue it
 nodesVector[startingNode].SetColour(Bayesnode::colour::BLACK);
 queue.push_back(startingNode);

 while(!queue.empty())
 {
  unsigned int dequeue_node = queue.front();
  std::cout << dequeue_node << " ";
  list_to_return.push_back(dequeue_node);
  queue.pop_front();

  // Get all adjacent vertices of the dequeued node.
  // If a adjacent has not been visited, then mark it visited and enqueue it.
  auto temp_list = ReturnOutEdges(dequeue_node);

  for(auto it = temp_list.begin(); it != temp_list.end(); ++it)
  {

   if(nodesVector[*it].GetColour() == Bayesnode::colour::WHITE){
    nodesVector[*it].SetColour(Bayesnode::colour::BLACK);
    queue.push_back(*it);
   }
  }
 }
 return list_to_return;
}

/**
* Depth First Search algorithm. Starting from a node it performs a depth-first traversal of the network.
*
* @param startingNode the index of the node
* @param spToList a shared pointer to a list, that is filled recursively by the algorithm.
* @param resetColour if true it reset all the colour to white before starting the algorithm
**/
void Bayesnet::DepthFirstSearch(unsigned int startingNode, std::shared_ptr<std::list<unsigned int>> spToList, bool resetColours ) {

 //Mark all nodes as white (not visited)
 if (resetColours == true) ResetAllColours();

 // Mark the current node as grey
 nodesVector[startingNode].SetColour(Bayesnode::colour::GREY);
  //std::cout << startingNode << " ";
   spToList->push_back(startingNode);

  // Get all adjacent vertices of the dequeued node.
  // If a adjacent has not been visited, then mark it visited and enqueue it.
  auto temp_list = ReturnOutEdges(startingNode);

  //Chain iteration to all the children
  for(auto it = temp_list.begin(); it != temp_list.end(); ++it)
  {
   if(nodesVector[*it].GetColour() == Bayesnode::colour::WHITE){
     nodesVector[*it].SetColour(Bayesnode::colour::GREY);
     DepthFirstSearch(*it, spToList, false);
     //list_to_return.merge(temp_list);
   }
  }

 // Done Visiting StartingNode
 nodesVector[startingNode].SetColour(Bayesnode::colour::BLACK);
}



} //namespace















