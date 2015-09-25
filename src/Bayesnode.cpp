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
Bayesnode::Bayesnode(unsigned int numberOfStates = 2) : mConditionalTable(numberOfStates)
{
 if (numberOfStates <= 1) numberOfStates = 2; //the minimum number of states allowed is 2
 valuesVector.reserve(numberOfStates);

 //Bayesnode::statesNumber = numberOfStates;
 //std::vector<unsigned int> void_vector;
 //void_vector.reserve(1);
 
}

Bayesnode::~Bayesnode()
{
}

/**
* It returns the states associate with the node.
* 
* @return the number of states associated with the node
**/
unsigned int Bayesnode::ReturnNumberOfStates(){
 return statesNumber;
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

 //Adding the sp to the vector
 std::weak_ptr<Bayesnode> wp_temp = spNode;
 incomingVector.push_back(wp_temp);

 //Add the new states to the Conditional Table
 unsigned int states_to_add = spNode->statesNumber;
 mConditionalTable.AddParentState(states_to_add);

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




