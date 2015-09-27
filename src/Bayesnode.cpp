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
Bayesnode::Bayesnode(unsigned int numberOfStates = 2) : mEvidence(-1), mConditionalTable(numberOfStates), mNodeLabel("")
{

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
 //the number of states is obtained directly from the CPT
 return mConditionalTable.ReturnRowsNumber();
}

/**
* It sets the label associate with the node.
* 
* @param nodeLabel the string to associate with the label
**/
void Bayesnode::SetLabel(std::string nodeLabel){
 mNodeLabel = nodeLabel;
}

/**
* It returns the label associate with the node.
* 
* @return the label associated with the node
**/
std::string Bayesnode::GetLabel(){
 return mNodeLabel;
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
 unsigned int states_to_add = spNode->ReturnNumberOfStates();
 mConditionalTable.AddVariable(states_to_add);

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
* Removing an incoming connection from this node.
* 
* @param spNode shared_ptr to the outgoing node
* @return it returns true if the element was found and erased
**/
bool Bayesnode::RemoveIncomingConnection(std::shared_ptr<Bayesnode> spNode){
 auto it_to_remove = incomingVector.end();

 for(auto it=incomingVector.begin(); it != incomingVector.end(); ++it){
  if ((*it).lock() == spNode){
   it_to_remove = it; 
  }
 }

 if(it_to_remove != incomingVector.end()){
  incomingVector.erase(it_to_remove);
  return true;
 }else{
  return false;
 }
}

/**
* Removing an outgoing connection from this node.
* 
* @param spNode shared_ptr to the outgoing node
* @return it returns true if the element was found and erased
**/
bool Bayesnode::RemoveOutgoingConnection(std::shared_ptr<Bayesnode> spNode){
 auto it_to_remove = outgoingVector.end();

 for(auto it=outgoingVector.begin(); it != outgoingVector.end(); ++it){
  if ((*it).lock() == spNode){
   it_to_remove = it; 
  }
 }

 if(it_to_remove != outgoingVector.end()){
  outgoingVector.erase(it_to_remove);
  return true;
 }else{
  return false;
 }
}

/**
* It search inside the incoming and outgoing containers if there are
* pointers to deleted nodes. If the wrong references are found they are deleted.
* 
* @return it returns the number of expired nodes deleted.
**/
unsigned int Bayesnode::EraseExpiredConnections(){
 unsigned int counter = 0;
 auto it = incomingVector.begin();
 //Iterating through the vector and looking for expired weak_ptr
 //vector.erase() returns a pointer to the new location of the element that followed the last element erased
 while( it != incomingVector.end() ) {
  if( it->expired() ){
   it = incomingVector.erase(it); 
   counter++;
  }
  ++it;
 }

 it = outgoingVector.begin();
 while( it != outgoingVector.end() ) {
  if( it->expired() ){
   it = outgoingVector.erase(it);
   counter++;
  }
  ++it;
 }

 return counter;
}

/**
* A node is an evidence when is outcome state is given.
*
* @param evidenceState the state to set as evidence.
* @return it returns true if the state was correctly set.
* 
**/
bool Bayesnode::SetAsEvidence(unsigned int evidenceState){
 if(evidenceState < ReturnNumberOfStates()){
  mEvidence = evidenceState;
  return true;
 }else{
  return false;
 }
}


/**
* A node is an evidence when is outcome state is given.
* 
* @return it returns -1 if the node is not an evidence.
* Otherwise it return the state sets as evidence.
**/
int Bayesnode::IsEvidence(){
 return mEvidence;
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

/**
* It checks if a given node is a parent.
*
* @param spNode a shared pointer to a Bayes node. 
* @return it returns true if the node is a parent node.
**/
bool Bayesnode::IsParent(std::shared_ptr<Bayesnode> spNode){
 for(auto it=incomingVector.begin(); it != incomingVector.end(); ++it){
  //check if the weak_ptr is expired
  if ((*it).expired() == true){
   return false;
  }

  if ((*it).lock() == spNode) return true; 
 }

 return false;
}

/**
* It checks if a given node is a child.
*
* @param spNode a shared pointer to a Bayes node. 
* @return it returns true if the node is a child node.
**/
bool Bayesnode::IsChild(std::shared_ptr<Bayesnode> spNode){
 for(auto it=outgoingVector.begin(); it != incomingVector.end(); ++it){
  //check if the weak_ptr is expired
  if ((*it).expired() == true){
   return false;
  }

  if ((*it).lock() == spNode) return true; 
 }

 return false;
}


}//namespace




