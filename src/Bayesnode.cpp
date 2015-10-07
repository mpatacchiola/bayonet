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


/**
* The constructor of the object.
*
* @param numberOfStates the number of discrete state to assign to this node
* the minimum number of states allowed is 2. The values assigned to the states are
* randomly generated and normalized.
**/
Bayesnode::Bayesnode(unsigned int numberOfStates) : conditionalTable(numberOfStates), mEvidence(-1), mCurrentColour(colour::WHITE), mNumberOfStates(numberOfStates), mNodeLabel(""), mNodeNumericLabel(0)
{
}

Bayesnode::~Bayesnode()
{
}

/**
* It returns the number of states associated with the node.
* 
* @return the number of states associated with the node
**/
unsigned int Bayesnode::ReturnNumberOfStates(){
 return mNumberOfStates;
}

/**
* It sets the label associated with the node.
* 
* @param nodeLabel the string to associate with the label
**/
void Bayesnode::SetLabel(std::string nodeLabel){
 mNodeLabel = nodeLabel;
}

/**
* It returns the label associated with the node.
* 
* @return the label associated with the node
**/
std::string Bayesnode::GetLabel(){
 return mNodeLabel;
}

/**
* It sets the numeric label of the node.
* 
* @return the label associated with the node
**/
void Bayesnode::SetNumericLabel(int numericLabel){
 mNodeNumericLabel = numericLabel;
}

/**
* It returns the numeric label associated with the node.
* 
* @return the numeric label associated with the node
**/
int Bayesnode::GetNumericLabel(){
 return mNodeNumericLabel;
}

/**
* It sets the colour of the node.
* The colour is used in searching algorithms.
* White vertices are undiscovered. 
* Grey is an adjacent vertex that is not already discovered.
* Black vertices are discovered and are adjacent to only other black or gray vertices.
* 
**/
void Bayesnode::SetColour(colour colourToSet){
 mCurrentColour = colourToSet;
}

/**
* It return the colour associated with the node.
* 
**/
Bayesnode::colour Bayesnode::GetColour(){
 return mCurrentColour;
}

/**
* Adding an index to the adjacency list of the node.
* 
* @param index
**/
bool Bayesnode::AddToAdjacencyList(unsigned int index){

 //looking for copies
 for(auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
  if( *it == index) return false;
 }
 adjacencyList.push_back(index);
 //conditionalTable.AddVariable(totStates);

 return true;
}


/**
* Removing an index to the adjacency list of the node.
* 
* @param index
* @return it returns true if the element was found and erased
**/
bool Bayesnode::RemoveFromAdjacencyList(unsigned int index){
 bool result = false;
 //looking if present
 for(auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
  if( *it == index) result = true;;
 }
 adjacencyList.remove(index);
 return result;
}


/**
* It returns a const reference to the internal adjacency list.
*
* @return a const reference to the list
* 
**/
const std::list<unsigned int>& Bayesnode::ReturnAdjacencyList(){
 return adjacencyList;
}

/**
* It looks for the index specified in input inside the adjacency list.
*
* @param index 
* @return true if the element is inside the list
* 
**/
bool Bayesnode::IsInAdjacencyList(unsigned int index){
  //looking for edges
 for(auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
  if( *it == index) return  true;
 }
 return false;
}

/**
* It returns the number of Incoming edges
*
* @return 
* 
**/
unsigned int Bayesnode::SizeOfAdjacencyList(){
 return adjacencyList.size();
}

/**
* A node is an evidence when is outcome state is given.
* 
* @return it returns -1 if the node is not an evidence.
* Otherwise it return the state sets as evidence.
**/
bool Bayesnode::IsEvidence(){
 if(mEvidence == -1) return false;
 else return true;
}

/**
* A node is an evidence when is outcome state is given.
*
* @param evidenceState the state to set as evidence.
* @return it returns true if the state was correctly set.
* 
**/
bool Bayesnode::SetEvidence(unsigned int evidenceState){
 if(evidenceState < ReturnNumberOfStates()){
  mEvidence = evidenceState;
  return true;
 }else{
  return false;
 }
}

/**
* It returns the evidence. If the node is not an evidence node it returns -1
*
* @return it returns true if the state was correctly set.
* 
**/
unsigned int Bayesnode::GetEvidence(){
 return mEvidence;
}


}//namespace




