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
Bayesnet::Bayesnet(std::vector<unsigned int> nodesTotStatesVector) : jointTable(nodesTotStatesVector) {
 //std::vector<unsigned int> tot_states_vector;
 //tot_states_vector.reserve(numberOfNodes);
 nodesVector.reserve(nodesTotStatesVector.size());
 for(auto it=nodesTotStatesVector.begin(); it!=nodesTotStatesVector.end(); ++it){
  auto sp = std::make_shared<Bayesnode>(*it);
  nodesVector.push_back(sp); //filling the nodes vector
 }

 //for(unsigned int i=0; i<numberOfNodes; i++){
  //auto sp = std::make_shared<Bayesnode>(numberOfStates);
  //sp->SetNumericLabel(i); //applying an incremental numeric label
  //nodesVector.push_back(sp); //filling the nodes vector
  //tot_states_vector.push_back(numberOfStates); //building the vector to give to the JPT
 //}
 //auto sp_to_set = std::make_shared<JointProbabilityTable>(tot_states_vector);
 //spJointTable = sp_to_set;
}

/**
* It destroys the object.
*
**/
Bayesnet::~Bayesnet(){}

/**
* Operator overload [] it is used to return the smart pointer reference to the node stored inside the net
* It is possible to access the methods of the single node using the deferencing operator ->
* Example: net[2]->IsRoot();  // It checks if the third node is a root node
* @param index the number of the element stored inside the net
* @return it returns a shared_ptr to the node
**/
std::shared_ptr<Bayesnode> Bayesnet::operator[](unsigned int index){
 if (index >= nodesVector.size()) throw std::domain_error("Error: Out of Range index.");
 return nodesVector[index];
}

/**
* It add a Edge between two nodes.
*
* @param firstNode the parent node
* @param secondNode the child node
**/
bool Bayesnet::AddEdge(unsigned int firstNode, unsigned int secondNode){
 if(firstNode == secondNode) return false;
 unsigned int node_states = nodesVector[firstNode]->ReturnNumberOfStates();
 //nodesVector[secondNode]->AddIncomingEdge(firstNode, node_states);
 auto sp_first = nodesVector[firstNode];
 auto sp_second = nodesVector[secondNode];
 unsigned int first_tot_states = sp_first->ReturnNumberOfStates();
 sp_first->AddToAdjacencyList(secondNode);
 sp_second->conditionalTable.AddVariable(first_tot_states);
 return true;
}

/**
* It remove an Edge between two nodes.
*
* @param firstNode the parent node
* @param secondNode the child node
**/
bool Bayesnet::RemoveEdge(unsigned int firstNode, unsigned int secondNode){
 if(firstNode == secondNode) return false;
 auto sp_first = nodesVector[firstNode];
 sp_first->RemoveFromAdjacencyList(secondNode);
 return true;
}

/**
* It checks if an Edge between two nodes exist.
*
* @param firstNode the parent node
* @param secondNode the child node
**/
bool Bayesnet::HasEdge(unsigned int FirstNode, unsigned int SecondNode){
 //return nodesVector[SecondNode]->HasIncomingEdgeFrom(FirstNode);
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
 unsigned int tot_edges;
 for(auto it=nodesVector.begin(); it!=nodesVector.end(); ++it){
  tot_edges += (*it)->ReturnAdjacencyList().size();
 }

 return tot_edges;
}

/**
* It returns the average dimensions of the Markov blanket for all the nodes inside the network.
*
**/
double Bayesnet::ReturnAverageMarkovBlanketSize(){
 //double average_size = 0;
 //for(auto it=nodesVector.begin(); it!=nodesVector.end(); ++it){
 // average_size += (*it)->ReturnMarkovBlanketSize(); //summing all the markov size for all nodes
 //}
 //return average_size / (double) nodesVector.size(); //dividing by the total number of nodes
}

/**
* It returns output index list
*
**/
std::list<unsigned int> Bayesnet::ReturnOutEdges(unsigned int index){
 std::list<unsigned int> temp_list;
 temp_list = nodesVector[index]->ReturnAdjacencyList();
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
  if((*it)->HasOutgoingEdgeTo(index) == true) temp_list.push_back(counter);
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
* It returns the chain of parents from this node to the root.
* It is basically a Reverse Breadth First Search.
*
**/
std::list<unsigned int> Bayesnet::ReturnParentChain(unsigned int index){
//Mark all nodes as white (not visited)
  ResetAllColours();

 // Create a queue for BFS
 std::list<int> queue;

 // Mark the current node as visited and enqueue it
 unsigned int startingNode = index;
 nodesVector[startingNode]->SetColour(Bayesnode::colour::BLACK);
 queue.push_back(startingNode);
 unsigned int dequeue_node = startingNode;
 std::list<unsigned int> cronology_list;

 while(!queue.empty())
 {
  dequeue_node = queue.front();
  std::cout << dequeue_node << " ";
  queue.pop_front();

  // Get all adjacent vertices of the dequeued node.
  // If a adjacent has not been visited, then mark it visited and enqueue it.
  auto temp_list = ReturnInEdges(dequeue_node);

  for(auto it = temp_list.begin(); it != temp_list.end(); ++it)
  {

   if(nodesVector[*it]->GetColour() != Bayesnode::colour::BLACK){
    nodesVector[*it]->SetColour(Bayesnode::colour::BLACK);
    queue.push_back(*it);  
    cronology_list.push_front(*it); //here the chain is registered  
   }
   
  }
 }

 return cronology_list;
}

/**
* It reset all colours for all nodes to white.
*
**/
void Bayesnet::ResetAllColours(){

  Bayesnode::colour my_colour = Bayesnode::colour::WHITE;
  for(auto it=nodesVector.begin(); it!=nodesVector.end(); ++it){
   (*it)->SetColour(my_colour);
  }
}

/**
* A root node is a node without parents.
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
* A leaf node is a node without children.
* 
* @return it returns true if the node is a leaf node.
**/
bool Bayesnet::IsLeaf(){
return false;
}

const std::vector<std::shared_ptr<Bayesnode>>& Bayesnet::ReturnNodesVector(){
 return nodesVector;
}


/**
* It fills the Joint Probability Table using the data from the Bayesian network.
*
* @return it returns a smart pointer to the Joint Table
**/
void Bayesnet::FillJointProbabilityTable(){

 //cycle through the key of the map
 for(auto it_map = jointTable.ReturnJointMap().begin(); it_map != jointTable.ReturnJointMap().end(); ++it_map){
  std::vector<unsigned int> variable_states_vector = it_map->first;
  std::vector<double> probability_vector;
  unsigned int nodes_counter = 0;
  //cycle through all the nodes
  for(auto it_node=nodesVector.begin(); it_node!=nodesVector.end(); ++it_node){
   unsigned int node_state = variable_states_vector[nodes_counter];
   std::vector<unsigned int> conditional_key_vector;
   //cycle through Adjacency List
   //for ( auto it_list = (*it_node)->ReturnAdjacencyList().begin(); it_list !=(*it_node)->ReturnAdjacencyList().end(); ++it_list){
   std::list<unsigned int> temp_in_list = ReturnInEdges(nodes_counter);
   for ( auto it_list = temp_in_list.begin(); it_list != temp_in_list.end(); ++it_list){
    conditional_key_vector.push_back( variable_states_vector.at(*it_list) );
    //std::cout << variable_states_vector.at(*it_list) << "-";
   }
   double local_probability = (*it_node)->conditionalTable.GetProbability(node_state, conditional_key_vector);
   probability_vector.push_back(local_probability);
   nodes_counter++;
  }
  double global_probability = 1; //TO 1 because of the multiplication chain
  //cycle through local probability vector
  for(auto it_prob = probability_vector.begin(); it_prob!= probability_vector.end(); ++it_prob){
   global_probability *= *it_prob;
  }
  //Assign the value to the Joint Table
  jointTable.SetProbability(variable_states_vector , global_probability);
 }
}

/**
* Breadth First Search algorithm. Starting from a node it performs a breadth-first traversal of the network.
* It is possible to associate a function to the algorithm. The function is called when a node is marked as visited (black colour).
*
* @param startingNode the index of the node 
* @param functionNode
**/
void Bayesnet::BreadthFirstSearch(unsigned int startingNode, std::function<void(Bayesnode)> functionNode){
 // bread-first-search on the Bayesian network
    // Mark all the vertices as not visited
    unsigned int V = nodesVector.size();
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
        visited[i] = false;
 
    // Create a queue for BFS
    std::list<int> queue;
 
    // Mark the current node as visited and enqueue it
    visited[startingNode] = true;
    queue.push_back(startingNode);
 
    // 'i' will be used to get all adjacent vertices of a vertex
    //std::list<int>::iterator it;
 
    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        startingNode = queue.front();
        std::cout << startingNode << " ";
        queue.pop_front();
 
        // Get all adjacent vertices of the dequeued vertex startingNode
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        for(auto it = ReturnOutEdges(startingNode).begin(); it != ReturnOutEdges(startingNode).end(); ++it)
        {
            if(!visited[*it])
            {
                visited[*it] = true;
                queue.push_back(*it);
            }
        }
    }
}


void Bayesnet::BreadthFirstSearch(unsigned int startingNode){

 //Mark all nodes as white (not visited)
  ResetAllColours();

 // Create a queue for BFS
 std::list<int> queue;

 // Mark the current node as visited and enqueue it
 nodesVector[startingNode]->SetColour(Bayesnode::colour::BLACK);
 queue.push_back(startingNode);

 while(!queue.empty())
 {
  unsigned int dequeue_node = queue.front();
  std::cout << dequeue_node << " ";
  queue.pop_front();

  // Get all adjacent vertices of the dequeued node.
  // If a adjacent has not been visited, then mark it visited and enqueue it.
  auto temp_list = ReturnOutEdges(dequeue_node);

  for(auto it = temp_list.begin(); it != temp_list.end(); ++it)
  {

   if(nodesVector[*it]->GetColour() == Bayesnode::colour::WHITE){
    nodesVector[*it]->SetColour(Bayesnode::colour::BLACK);
    queue.push_back(*it);
   }
  }
 }
}

void Bayesnet::DepthFirstSearch(unsigned int startingNode, bool resetColours ){

 //Mark all nodes as white (not visited)
 if (resetColours == true) ResetAllColours();

 // Mark the current node as grey
 nodesVector[startingNode]->SetColour(Bayesnode::colour::GREY);
  std::cout << startingNode << " ";

  // Get all adjacent vertices of the dequeued node.
  // If a adjacent has not been visited, then mark it visited and enqueue it.
  auto temp_list = ReturnOutEdges(startingNode);

  //Chain iteration to all the children
  for(auto it = temp_list.begin(); it != temp_list.end(); ++it)
  {
   if(nodesVector[*it]->GetColour() == Bayesnode::colour::WHITE){
     nodesVector[*it]->SetColour(Bayesnode::colour::GREY);
     DepthFirstSearch(*it, false);
   }
  }

 // Done Visiting StartingNode
 nodesVector[startingNode]->SetColour(Bayesnode::colour::BLACK);
}



} //namespace















