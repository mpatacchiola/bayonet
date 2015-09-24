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

#include "Bayesnode.h"
#include <random>
#include <iostream>
#include <algorithm> //std::find

namespace bayonet{

//Bayesnode::Bayesnode()
//{
//}

/**
* The constructor of the object.
*
* @param numberOfStates the number of discrete state to assign to this node
* the minimum number of states allowed is 2. The values assigned to the states are
* randomly generated and normalized.
**/
Bayesnode::Bayesnode(unsigned int numberOfStates = 2){
 if (numberOfStates <= 1) numberOfStates = 2; //the minimum number of states allowed is 2
 valuesVector.reserve(numberOfStates);

 Bayesnode::statesNumber = numberOfStates;

 std::random_device random_device;
 std::mt19937 generator(random_device());
 std::uniform_real_distribution<> real_dist(0, 1);
 double accumulator = 0;

 //generate random values and push them inside the state vector
 for (unsigned int i = 0; i < numberOfStates; i++) {
  double temp_value = real_dist(generator);
  valuesVector.push_back(temp_value);
  accumulator += temp_value;      
 }

 //normalize all the elements into the vector (they must sum to 1)
 for(auto it = valuesVector.begin(); it != valuesVector.end(); ++it) {
  *it = *it / accumulator;
 }
}

Bayesnode::~Bayesnode()
{
}

/**
* It prints the current values associated with the node states.
*
**/
void Bayesnode::PrintStates(){
 unsigned int counter = 0;
 std::cout << "Total States Number: " << valuesVector.size() << std::endl;
 for(auto it = valuesVector.begin(); it != valuesVector.end(); ++it) {
  std::cout << "State: " << counter << " ..... " << *it << std::endl;
  counter++;
 }
}

/**
* The function returns the number of states of the node.
*
* @return it returns the number of states
**/
unsigned int Bayesnode::ReturnStatesNumber(){
 return valuesVector.size();
}

double Bayesnode::ReturnStatesSum(){
 double accumulator = 0;

 //sum all the values inside the vector, to obtain the normalization constant
 for(auto it = valuesVector.begin(); it != valuesVector.end(); ++it) {
  accumulator += *it;
 }
 return accumulator;
}

/**
* The function returns the number of states of the node.
*
* @param index an unsigned integer representing the state index
* @param value a double representing the value to assign to the state
* @return it returns true if the value is correctly set
**/
bool Bayesnode::SetStateValue(unsigned int index, double value){
 valuesVector[index] = value;
 return true;
}

/**
* The function returns the number associated to a specific state value.
*
* @param index an unsigned integer representing the state index
* @return it returns the state value
**/
double Bayesnode::GetStateValue(unsigned int index){
 return valuesVector[index];
}

/**
* The function normalizes all the values inside the vector.
* The values assigned to the states must always sum to 1.
*
**/
void Bayesnode::NormalizeValues(){
 double accumulator = 0;

 //sum all the values inside the vector, to obtain the normalization constant
 for(auto it = valuesVector.begin(); it != valuesVector.end(); ++it) {
  accumulator += *it;
 }

 //normalize all the elements into the vector (they must sum to 1)
 for(auto it = valuesVector.begin(); it != valuesVector.end(); ++it) {
  *it = *it / accumulator;
 }
}

/**
* Adding an incoming connection to this node.
* 
* @param spNode shared_ptr to the incoming node
**/
bool Bayesnode::AddIncomingConnection(std::shared_ptr<Bayesnode> spNode){
 //no cycles allowed
 for(auto it = outgoingVector.begin(); it != outgoingVector.end(); ++it) {
  if((*it).lock() == spNode) return false;
 }

 //looking for copies
 for(auto it = incomingVector.begin(); it != incomingVector.end(); ++it) {
  if((*it).lock() == spNode) return false;
 }

 std::weak_ptr<Bayesnode> wp_temp = spNode;
 incomingVector.push_back(wp_temp);
 return true;
}

/**
* Adding an outgoing connection from this node.
* 
* @param spNode shared_ptr to the outgoing node
**/
bool Bayesnode::AddOutgoingConnection(std::shared_ptr<Bayesnode> spNode){
 //no cycles allowed
 for(auto it = incomingVector.begin(); it != incomingVector.end(); ++it) {
  if((*it).lock() == spNode) return false;
 }

 //looking for copies
 for(auto it = outgoingVector.begin(); it != outgoingVector.end(); ++it) {
  if((*it).lock() == spNode) return false;
 }

 std::weak_ptr<Bayesnode> wp_temp = spNode;
 outgoingVector.push_back(wp_temp);
 return true;
}

/**
* This function get a sample from the distribution associated with the node.
* 
* @return it returns the state sampled.
**/
unsigned int Bayesnode::ReturnSample(){
 std::default_random_engine generator;
 std::discrete_distribution<int> values_distribution (valuesVector.begin(), valuesVector.end());
 return values_distribution(generator);
}

/**
* A root node is a node without parents.
* 
* @return it returns true if the node is a root node.
**/
bool Bayesnode::IsRoot(){
 if(incomingVector.size() == 0) return true;
 else return false;
}

/**
* A leaf node is a node without children.
* 
* @return it returns true if the node is a leaf node.
**/
bool Bayesnode::IsLeaf(){
 if(outgoingVector.size() == 0) return true;
 else return false;
}

}




