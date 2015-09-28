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

namespace bayonet{

/**
* It create a net with a certain number of nodes and number of states.
*
* @param numberOfNodes the number of nodes to add to the network
* @param numberOfStates the number of state to assign to each node
**/
Bayesnet::Bayesnet(unsigned int numberOfNodes, unsigned int numberOfStates = 2){
 std::vector<unsigned int> tot_states_vector;
 tot_states_vector.reserve(numberOfNodes);
 for(unsigned int i=0; i<numberOfNodes; i++){
  auto sp = std::make_shared<Bayesnode>(numberOfStates);
  sp->SetNumericLabel(i); //applying an incremental numeric label
  nodesVector.push_back(sp); //filling the nodes vector
  tot_states_vector.push_back(numberOfStates); //building the vector to give to the JPT
 }
 auto sp_to_set = std::make_shared<JointProbabilityTable>(tot_states_vector);
 spJointTable = sp_to_set;
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

 auto edge_pair = std::make_pair (firstNode,secondNode);
 auto it_to_check = edgesVector.end();
 for(auto it=edgesVector.begin(); it!=edgesVector.end(); ++it){
  if((*it) == edge_pair) it_to_check = it;
 }

 if(it_to_check == edgesVector.end()){
  edgesVector.push_back(edge_pair);
  nodesVector[firstNode]->AddOutgoingEdge(nodesVector[secondNode]); 
  nodesVector[secondNode]->AddIncomingEdge(nodesVector[firstNode]);
  return true;
 }else{
  return false;
 }
 edgesVector.push_back(std::make_pair (firstNode,secondNode));
 return true;
}

/**
* It remove a Edge between two nodes.
*
* @param firstNode the parent node
* @param secondNode the child node
**/
bool Bayesnet::RemoveEdge(unsigned int firstNode, unsigned int secondNode){
 if(firstNode == secondNode) return false;
 
 auto edge_pair = std::make_pair (firstNode,secondNode);
 auto it_to_remove = edgesVector.end();

 for(auto it=edgesVector.begin(); it!=edgesVector.end(); ++it){
  if((*it) == edge_pair) it_to_remove = it;
 }

 if(it_to_remove == edgesVector.end()){
  return false;
 }else{
  edgesVector.erase(it_to_remove);
  nodesVector[firstNode]->RemoveOutgoingEdge(nodesVector[secondNode]); 
  nodesVector[secondNode]->RemoveIncomingEdge(nodesVector[firstNode]);
  return true;
 }

}

/**
* It returns the number of nodes.
*
**/
unsigned int Bayesnet::ReturnNumberOfNodes(){
 return  nodesVector.size();
}

/**
* It returns the number of edges.
*
**/
unsigned int Bayesnet::ReturnNumberOfEdges(){
 return edgesVector.size();
}

/**
* It returns the average dimensions of the Markov blanket for all the nodes inside the network.
*
**/
double Bayesnet::ReturnAverageMarkovBlanketSize(){
 double average_size = 0;
 for(auto it=nodesVector.begin(); it!=nodesVector.end(); ++it){
  average_size += (*it)->ReturnMarkovBlanketSize(); //summing all the markov size for all nodes
 }
 return average_size / (double) nodesVector.size(); //dividing by the total number of nodes
}

/**
* It fills the Joint Probability Table using the data from the Bayesian network.
*
* @return it returns a smart pointer to the Joint Table
**/
std::shared_ptr<JointProbabilityTable> Bayesnet::FillJointProbabilityTable(){

 std::vector<unsigned int> variable_states_vector;
 for(unsigned int i=0; i<spJointTable->ReturnRowsNumber(); i++){
  variable_states_vector = spJointTable->ReturnKey(i);
 }


 //1- It returns the key from the JPT

 //2- The key is the state of each node
 // there is a one-to-one association node-state(key)

 //3- It is necessary to buld a query to send to each CPT of each node
 // for loop scanning each node, then creating a vector(query) for each node
 // the vector(query) can be filled using the edgesVector cronology.

 //QUERY(variableState, {parents state})
 

 return spJointTable;
}

} //namespace




