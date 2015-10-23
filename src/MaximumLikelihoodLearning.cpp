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

#include"MaximumLikelihoodLearning.h"
#include <iostream>
#include <random>
#include <list>
#include <utility>      // std::pair

namespace bayonet{


MaximumLikelihoodLearning::MaximumLikelihoodLearning(){}

MaximumLikelihoodLearning::~MaximumLikelihoodLearning(){}


/**
* It uses the training data to update the Conditional Probability
* Tables of each node in the network.
*
* @param net the Bayesian network to update
* @param trainingDataset a reference to a vector of vector (matrix)
* containing the state of each node obtained during the training phase.
* Each line of the matrix must have the same number of values of the 
* network given as input.
* @return it return the updated network
*
**/
Bayesnet MaximumLikelihoodLearning::ReturnUpdatedNetwork(Bayesnet net, std::vector<std::vector<unsigned int>>& trainingDataset){

 //1- For each node, Reset to zero the CPTs
 //There are states that are never learnt during
 //the training phase, in this case it is better
 //to initialize the value of the conditionalTable to 1.0
 //and add the dataset sample to them.
 //"Artificial Intelligence: A Modern Approach." chapter 17
 unsigned int tot_nodes = net.ReturnNumberOfNodes();
 for(unsigned int i=0; i<tot_nodes; i++){
  net[i].conditionalTable.ResetProbabilities(1.0); //<--- TRICK from the book 
 }

 //2- For each training vecotr, For each node, take the conditional table, 
 //create a key given the input edges, add +1 to the entry in the table.
 for(auto it_data = trainingDataset.begin(); it_data!=trainingDataset.end(); ++it_data){
  auto sample_vector = *it_data;
  //Iterating all the nodes
  for(unsigned int i=0; i<tot_nodes; i++){
   unsigned int node_state = sample_vector[i];
   auto in_list = net.ReturnInEdges(i);
   std::vector<unsigned int> key_vector;
   key_vector.reserve(in_list.size());
   //Creating the key
   for(auto it_in=in_list.begin(); it_in!=in_list.end(); ++it_in){
    key_vector.push_back(sample_vector[*it_in]);
   }
   //Key vector complete, adding the +1 to the probability 
   net[i].conditionalTable.AddToProbability(node_state, key_vector, 1.0);
  }
 }

 //3- For each node, normalize the CPT
 for(unsigned int i=0; i<tot_nodes; i++){
  net[i].conditionalTable.NormalizeProbabilities();
 }

 //4- return the updated network
 return net;
}


} //namespace












