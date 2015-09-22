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
Bayesnode::Bayesnode(unsigned int numberOfStates = 2, std::string label = ""){
 if (numberOfStates <= 1) numberOfStates = 2; //the minimum number of states allowed is 2
 statesVector.reserve(numberOfStates);
 nodeLabel = label; //assign the name

 std::random_device random_device;
 std::mt19937 generator(random_device());
 std::uniform_real_distribution<> real_dist(0, 1);
 double accumulator = 0;

 //generate random values and push them inside the state vector
 for (unsigned int i = 0; i < numberOfStates; i++) {
  double temp_value = real_dist(generator);
  statesVector.push_back(temp_value);
  accumulator += temp_value;      
 }

 //normalize all the elements into the vector (they must sum to 1)
 for(auto it = statesVector.begin(); it != statesVector.end(); ++it) {
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
 std::cout << "Total States Number: " << statesVector.size() << std::endl;
 for(auto it = statesVector.begin(); it != statesVector.end(); ++it) {
  std::cout << "State: " << counter << " Value:" << *it << std::endl;
  counter++;
 }
}

/**
* The function returns the number of states of the node.
*
* @return it returns the number of states
**/
unsigned int Bayesnode::ReturnStatesNumber(){
 return statesVector.size();
}

double Bayesnode::ReturnStatesSum(){
 double accumulator = 0;

 //sum all the values inside the vector, to obtain the normalization constant
 for(auto it = statesVector.begin(); it != statesVector.end(); ++it) {
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
bool Bayesnode::SetState(unsigned int index, double value){
 statesVector[index] = value;
 return true;
}

/**
* The function returns the number associated to a specific state value.
*
* @param index an unsigned integer representing the state index
* @return it returns the state value
**/
double Bayesnode::GetState(unsigned int index){
 return statesVector[index];
}

/**
* The function normalizes all the values inside the vector.
* The values assigned to the states must always sum to 1.
*
**/
void Bayesnode::NormalizeStates(){
 double accumulator = 0;

 //sum all the values inside the vector, to obtain the normalization constant
 for(auto it = statesVector.begin(); it != statesVector.end(); ++it) {
  accumulator += *it;
 }

 //normalize all the elements into the vector (they must sum to 1)
 for(auto it = statesVector.begin(); it != statesVector.end(); ++it) {
  *it = *it / accumulator;
 }
}


}




