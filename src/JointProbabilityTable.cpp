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

#include <iostream>
#include <random>
#include"JointProbabilityTable.h"

namespace bayonet{

/**
* The constructor of the object.
*
**/
JointProbabilityTable::JointProbabilityTable(){

}

/**
* The constructor of the object.
*
* @param variablesTotStates this is a list that represents the number of states of each parent of the node
* If the node has 3 boolean states as parents the list is {2, 2, 2}
**/
JointProbabilityTable::JointProbabilityTable(std::vector<unsigned int> variablesTotStates){

 //Fill the map
 FillMap(variablesTotStates);

 //Set the Parents state vector
  mVariablesTotStatesVector = variablesTotStates;
}

/**
* Destroying the object
*
**/
JointProbabilityTable::~JointProbabilityTable(){}

/**
* It returns the specified row from the table
*
* @param index
**/
std::pair<std::vector<unsigned int>, double> JointProbabilityTable::ReturnRow(unsigned int index){
 std::vector<unsigned int> parent_vector;
 double probability;
 if(index > mJointMap.size()){
  std::cerr << "ERROR: Joint Table out of range index" << std::endl;
  return make_pair(parent_vector, probability); //out of range > empty vector returned
 }
 auto it_map=mJointMap.begin();
 std::advance(it_map, index);
 parent_vector = it_map->first;
 probability = it_map->second;
 return make_pair(parent_vector, probability);
}

/**
* It returns the marginal probability of a given variable.
*
* @param variableIndex
* @param variableState
**/
double JointProbabilityTable::ReturnMarginal(unsigned int variableIndex, unsigned int variableState){

  double total = 0;
  //Iterate through map content:
  for (auto it_map=mJointMap.begin(); it_map!=mJointMap.end(); ++it_map){
    if(it_map->first.at(variableIndex) == variableState) total += it_map->second;
  }
  return total;
}

/**
* It returns the key assciated with the index.
*
* @param index
**/
std::vector<unsigned int> JointProbabilityTable::ReturnKey(unsigned int index){
 unsigned int counter = 0;
 for(auto it=mJointMap.begin(); it!=mJointMap.end(); ++it){
  if(counter == index) return (*it).first;
  counter ++;
 }
 return {};
}

/**
* Given a vector key it returns the associated probabilities
*
* @param variablesTotStates
**/
double JointProbabilityTable::GetProbability(std::vector<unsigned int> variablesTotStates){
 return mJointMap[variablesTotStates];
}

/**
* Given a vector key it sets the probabilities associated
*
* @param variablesTotStates
* @param probabilities
**/
bool JointProbabilityTable::SetProbability(std::vector<unsigned int> variablesTotStates, double probability){
 if(mJointMap.find(variablesTotStates) != mJointMap.end()){
  mJointMap[variablesTotStates] = probability;
  //std::cout << "P: " << probability << std::endl;
  return true;
 }else{
  return false;
 }
}

bool JointProbabilityTable::AddToProbability(std::vector<unsigned int> variablesStatesVector, double valueToAdd){
 if(mJointMap.find(variablesStatesVector) != mJointMap.end()){
  mJointMap[variablesStatesVector] += valueToAdd;
  return true;
 }else{
  return false;
 }
}

/**
* It prints the conditional table on the terminal.
* If the number of columns and rows is huge, the terminal could cut parts of the output.
*
**/
void JointProbabilityTable::Print(){
 
  std::cout << std::endl;

 //std::cout << '|' << std::setw(5) << "KEY" << std::setw(5) << '|' << std::endl;

  //Iterate through map content:
  for (auto it_map=mJointMap.begin(); it_map!=mJointMap.end(); ++it_map){

   //THE KEY
   for (auto it_key=it_map->first.begin(); it_key!=it_map->first.end(); ++it_key){
    std::cout << (*it_key);
     if(it_key!=it_map->first.end()-1) std::cout << "-";
   }

   //THE DATA
   std::cout << " ..... " << it_map->second  << std::endl;
  }

 std::cout << std::endl;
 std::cout << "ROWS ..... " << JointProbabilityTable::ReturnRowsNumber() << std::endl;
 std::cout << std::endl;
}

/**
* It prints the probabilities associated with a particular set of parents.
* If the number of columns is huge, the terminal could cut parts of the output.
*
**/
void JointProbabilityTable::PrintProbability(std::vector<unsigned int> variablesTotStates){

 //THE KEY
 for (auto it=variablesTotStates.begin(); it!=variablesTotStates.end(); ++it){
  std::cout << *it;
  if(it!=variablesTotStates.end()-1) std::cout << "-";
 }

 //THE DATA
 std::cout << " ..... " << mJointMap[variablesTotStates]  << std::endl;
 std::cout << std::endl;
}

/**
* It prints the marginal probabilities associated with all the variables.
* If the number of columns is huge, the terminal could cut parts of the output.
*
**/
void JointProbabilityTable::PrintMarginals(){
 unsigned int variable_counter = 0;
 for (auto it=mVariablesTotStatesVector.begin(); it!=mVariablesTotStatesVector.end(); ++it){
  for(unsigned int i=0; i<*it; i++){
   double probability = ReturnMarginal(variable_counter, i);
   std::cout << "Variable: " << variable_counter << " State: " << i << " ..... " << probability << std::endl;
  }
  variable_counter++;
 } 
}

/**
* It prints the marginal probabilities associated with a specific variable.
* If the number of columns is huge, the terminal could cut parts of the output.
*
**/
void JointProbabilityTable::PrintMarginal(unsigned int variableIndex){
 unsigned int variableTotStates = mVariablesTotStatesVector[variableIndex];
  for(unsigned int i=0; i<variableTotStates; i++){
   double probability = ReturnMarginal(variableIndex, i);
   std::cout << "Variable: " << variableIndex << " State: " << i << " ..... " << probability << std::endl;
  }
}

/**
* It normalize the probabilities inside each row of the table.
*
**/
void JointProbabilityTable::NormalizeProbabilities(double alpha){
 //double accumulator = 0;

 //If the normalizing constant is less than zero it is not possible to divide
 //then it will be find thorugh iteration
 if(alpha <= 0){
  alpha = 0;
  //Iterate through map content:
  for (auto it_map=mJointMap.begin(); it_map!=mJointMap.end(); ++it_map){
    alpha += it_map->second;
  }
 }

 //Iterate through map content for the normalization
 for (auto it_map=mJointMap.begin(); it_map!=mJointMap.end(); ++it_map){
    it_map->second = it_map->second / alpha;
 }
}

/**
* It randomize the probabilities inside each row of the table.
* The probabilities are also normalized.
**/
void JointProbabilityTable::RandomizeProbabilities(){

 std::random_device random_device;
 std::mt19937 generator(random_device());
 std::uniform_real_distribution<> real_dist(0, 1);
 double accumulator = 0;

 //Iterate through map for adding the values
 for (auto it_map=mJointMap.begin(); it_map!=mJointMap.end(); ++it_map){

  //generate random values and push them inside the map
   double temp_value = real_dist(generator);
   it_map->second = temp_value;
   accumulator += temp_value;      
 }

 //Iterate through map for the normalization
 for (auto it_map=mJointMap.begin(); it_map!=mJointMap.end(); ++it_map){
  it_map->second = it_map->second / accumulator;
 }
}

/**
* It reset the probabilities inside each row of the table.
* Each value is set to zero.
**/
void JointProbabilityTable::ResetProbabilities(double valueToSet){
 //Iterate through map for the normalization
 for (auto it_map=mJointMap.begin(); it_map!=mJointMap.end(); ++it_map){
  it_map->second = valueToSet;
 }
}

/**
* It add a new variable to the table.
*
* @param totStates the total number of states to assign to the new variable
**/
void JointProbabilityTable::AddVariable(unsigned int totStates=2){

 //Check for wrong input
 if(totStates <2) totStates=2;

 //Push the new total in the vector
  mVariablesTotStatesVector.push_back(totStates);

 //clear the map
 mJointMap.clear();

 FillMap( mVariablesTotStatesVector);
}

/**
* It clear the content of the table.
*
**/
void JointProbabilityTable::Clear(){
 mJointMap.clear();
  mVariablesTotStatesVector.clear();
}

/**
* It returns the total number of rows.
*
**/
unsigned int JointProbabilityTable::ReturnRowsNumber(){
 return mJointMap.size();
}


/**
* Private function. It's a low level function for filling the content of the map.
* It works like an odometer and create all the possible combination of states starting from the parents states.
*
**/
void JointProbabilityTable::FillMap(std::vector<unsigned int> variablesTotStates){
 //0- In case of a root node without parents, it adds only one row
 if(variablesTotStates.size() == 0){
  return;
 }

 //1- Creation of the vector that contains the Subsets of unsigned int
 std::vector<std::vector<unsigned int>> vector_container; 
 for (auto it = variablesTotStates.begin() ; it != variablesTotStates.end() ; ++it)
 {
  std::vector<unsigned int> temp_vector;
  unsigned int total_states = (*it);
  for(unsigned int state_counter=0; state_counter<total_states; state_counter++){
   temp_vector.push_back(state_counter);
  }
  vector_container.push_back(temp_vector);
  }

 //2- Creation of the vector that contains the Iterators
 std::vector< std::vector<unsigned int>::iterator > iterator_container;
 for (auto it = vector_container.begin() ; it != vector_container.end() ; ++it){
   std::vector<unsigned int>::iterator temp_iterator;
   temp_iterator = (*it).begin();
   iterator_container.push_back(temp_iterator);
 }

 //3- filling the data value
 double data=0;

 //4- Cascade iteration for storing the whole set of combination
 unsigned int K = iterator_container.size();

 while (iterator_container[0] != vector_container[0].end()) {

    //Clear the key_vector before pushing a new key
    std::vector<unsigned int> key_vector;
    key_vector.reserve(variablesTotStates.size());

    //Filling the key_vector 
    for (auto it_key=iterator_container.begin(); it_key!=iterator_container.end(); ++it_key){    
     key_vector.push_back( (*(*it_key)) ); //pushing a new key
    }

    //Assign a new row in the conditional map
    mJointMap.insert( std::make_pair( key_vector, data ) );

   //Increment the odometer by 1 
   //ATTENTION: It must called at the end of the while otherwise the first key get missed...
   ++iterator_container[K-1]; 
   for (int i = K-1; (i > 0) && (iterator_container[i] == vector_container[i].end()); --i) {
    //subtracting the counter
    iterator_container[i] = vector_container[i].begin();
    ++iterator_container[i-1];
   }
 }
}

const std::map<std::vector<unsigned int>,double>& JointProbabilityTable::ReturnJointMap(){
 return mJointMap;
}


} //namespace


