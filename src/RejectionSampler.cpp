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

#include"RejectionSampler.h"
#include <iostream>
#include <random>
#include <list>
#include <utility>      // std::pair

namespace bayonet{


RejectionSampler::RejectionSampler(){}

RejectionSampler::~RejectionSampler(){}

/**
* It returns a single sample picking up it from the Bayesian network
*
* @param net the Bayesian network to use for picking up the sample.
*
**/
std::vector<unsigned int> RejectionSampler::ReturnSample(bayonet::Bayesnet& net){

 //Declaring the vector to return
 std::vector<unsigned int> vector_to_return;

 //Declaring the local variables
 auto topo_list = net.ReturnTopologicalList();
 std::map<unsigned int, std::pair<bool, unsigned int>> sample_map;
 unsigned int tot_nodes = net.ReturnNumberOfNodes();

 //Fill the sample_map and the vector_to_return with zeros
 for(unsigned int i=0; i<tot_nodes; i++){
  auto my_pair = std::make_pair<bool, unsigned int>(false,0);
  auto map_pair = std::make_pair(i, my_pair);
  sample_map.insert(map_pair);
  vector_to_return.push_back(0);
 }

 //Cycle through the topological list
 for(auto it_topo=topo_list.begin(); it_topo!=topo_list.end(); ++it_topo){
  auto in_list = net.ReturnInEdges(*it_topo);
  std::vector<unsigned int> key_vector;
  //Cycle through the in list for creating the key
  for(auto it_in=in_list.begin(); it_in!=in_list.end(); ++it_in){
   auto map_value = sample_map[*it_in]; //return the pair stored inside the map
   bool value_check = map_value.first; //return the boolean of the pair
   unsigned int value_sample = map_value.second; //return the unsigned int of the pair
   if(value_check == false){
    std::cout << "EXCEPTION: the topological order was not respected!" << std::endl;    
   }else{
    key_vector.push_back(value_sample); //push   
   }
  }
  //Key completed, asking for the sample
  auto sp_to_node = net[*it_topo];
  //Storing the sample in the local index vector
  std::pair<bool,unsigned int> pair_to_store = std::make_pair<bool, unsigned int>(true, sp_to_node->conditionalTable.ReturnSample(key_vector));
  sample_map[*it_topo] = pair_to_store;
  //Storing the sample into the vector to return
  //vector_to_return.push_back(pair_to_store.second);
 }

 //Fill the vector_to_return with the result stored inside the map
 //This cycle restore the nodes order (not the topological order)
 for(unsigned int i=0; i<tot_nodes; i++){
  auto temp_pair = sample_map[i];
  vector_to_return[i] = temp_pair.second; //Storing the sample into the vector to return
 }

 return vector_to_return;
}

/**
* It accumulate samples picking up them from the Bayesian network.
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
*
**/
std::vector<std::vector<unsigned int>> RejectionSampler::AccumulateSamples(Bayesnet& net, unsigned int cycles){
 std::vector<std::vector<unsigned int>> vector_to_return;
 for(unsigned int i=0; i<cycles; i++){
  vector_to_return.push_back(ReturnSample(net));
 }
 return vector_to_return;
}

/**
* It accumulates samples picking up them from the Bayesian network.
* It discards the sample that are not coerent with the evidence, the output vector
* contains only the accepted samples.
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
*
**/
std::vector<std::vector<unsigned int>> RejectionSampler::AccumulateAndDiscardSamples(Bayesnet& net, unsigned int cycles){
 std::vector<std::vector<unsigned int>> vector_to_return;
 unsigned int tot_nodes = net.ReturnNumberOfNodes();

 //Cycle for accumulating samples
 for(unsigned int i=0; i<cycles; i++){
  auto sample_vector = ReturnSample(net);
  bool discard = false;
  
  //Check all the values and if an evidence is lost it discards the vector
  for(unsigned int nodes_counter=0; nodes_counter<tot_nodes; nodes_counter++){
   if(net[nodes_counter]->IsEvidence() == true && net[nodes_counter]->GetEvidence() != sample_vector[nodes_counter]){
    discard = true;
    break; //stop the for loop
   }
  }

  //Discard is false, then I push the sample inside the output_vector
  if(discard == false)   vector_to_return.push_back(sample_vector);
 }
 return vector_to_return;
}

/**
* It prints the result of the sampling.
* It is possible to do it for different iterations.
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
*
**/
void RejectionSampler::PrintSample(bayonet::Bayesnet& net, unsigned int cycles){

 //Declaring the local variables
 auto topo_list = net.ReturnTopologicalList();
 std::map<unsigned int, std::pair<bool, unsigned int>> sample_map;
 unsigned int tot_nodes = net.ReturnNumberOfNodes();
 unsigned int correct_i_cycle = 0;

//Cycle for printing
for (unsigned int i_cycle=0; i_cycle<cycles; i_cycle++){
 correct_i_cycle = i_cycle + 1;
 if(correct_i_cycle <= 9) std::cout << correct_i_cycle << " ...... ";
 else if (correct_i_cycle > 9 && correct_i_cycle <= 99) std::cout << correct_i_cycle << " ..... ";
 else if (correct_i_cycle > 99 && correct_i_cycle <= 999) std::cout << correct_i_cycle << " .... ";
 else std::cout << correct_i_cycle << " ... ";

 //Clear the map at the beginning of the cycle
 sample_map.clear();

 //Fill the outcome vector with zeros
 for(unsigned int i=0; i<tot_nodes; i++){
  auto my_pair = std::make_pair<bool, unsigned int>(false,0);
  auto map_pair = std::make_pair(i, my_pair);
  sample_map.insert(map_pair);
 }

 //Cycle through the topological list
 for(auto it_topo=topo_list.begin(); it_topo!=topo_list.end(); ++it_topo){
  auto in_list = net.ReturnInEdges(*it_topo);
  std::vector<unsigned int> key_vector;
  //Cycle through the in list for creating the key
  for(auto it_in=in_list.begin(); it_in!=in_list.end(); ++it_in){
   auto map_value = sample_map[*it_in]; //return the pair stored inside the map
   bool value_check = map_value.first; //return the boolean of the pair
   unsigned int value_sample = map_value.second; //return the unsigned int of the pair
   if(value_check == false){
    std::cout << "EXCEPTION: the topological order was not respected!" << std::endl;    
   }else{
    key_vector.push_back(value_sample); //push   
   }
  }
  //Key completed, asking for the sample
  auto sp_to_node = net[*it_topo];
  //Storing the sample in the local index vector
  std::pair<bool,unsigned int> pair_to_store = std::make_pair<bool, unsigned int>(true, sp_to_node->conditionalTable.ReturnSample(key_vector));
  sample_map[*it_topo] = pair_to_store;
  //Storing the sample into the vector to return
  //std::cout << pair_to_store.second << " ";
 }

 //Printing the samples in the nodes order (not in the topological one)
 for(auto it_map = sample_map.begin(); it_map!=sample_map.end(); ++it_map){
  auto temp_pair = it_map->second;
  std::cout << temp_pair.second << " ";
 }
 std::cout << std::endl;

}
}

/**
* It creates a Joint Probability table starting from the Bayesian network and sampling for
* the number of iterations specified.
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
* @return it return a Joint Probability Table object
**/
JointProbabilityTable RejectionSampler::ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles){

 //Init variables
 std::map<std::vector<unsigned int>, unsigned int> counter_map;
 auto samples_vector = AccumulateAndDiscardSamples(net, cycles);

 //For creating the Joint Probability Table we need to knwo the tot states number of each node

 JointProbabilityTable joint_table(net.ReturnTotalStates());
 
 //It fills the map incrementing by 1 the value associated with a key
 //the key is a specific combination of states
 for(auto it_sample=samples_vector.begin(); it_sample!=samples_vector.end(); ++it_sample){
  counter_map[*it_sample] += 1;
 }

 //Iteration through the map for calculating the probabilities
 double norm = samples_vector.size(); //the normalization value is the number of elements inside the sample vector
 for(auto it_map=counter_map.begin(); it_map!=counter_map.end(); ++it_map){
  std::vector<unsigned int> key_vector = it_map->first;
  double probability = (double) it_map->second / norm;
  joint_table.SetProbability(key_vector, probability);
 }

 return joint_table;
}



} //namespace



