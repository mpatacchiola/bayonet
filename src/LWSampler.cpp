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

#include"LWSampler.h"
#include <iostream>
#include <random>
#include <list>
#include <utility>      // std::pair

namespace bayonet{


LWSampler::LWSampler(){}

LWSampler::~LWSampler(){}

/**
* It returns a std::pair containing a single sample picked up 
* from the Bayesian network and the relative weight.
*
* @param net the Bayesian network to use for picking up the sample.
*
**/
std::pair< std::vector<unsigned int>, double> LWSampler::ReturnSample(bayonet::Bayesnet& net){

 //Declaring the vector to return
 std::vector<unsigned int> vector_to_return;

 //Declaring the weight
 double weight_to_return = 1.0;

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
    std::cerr << "EXCEPTION: the topological order was not respected!" << std::endl;    
   }else{
    key_vector.push_back(value_sample); //push   
   }
  }
  //Key completed, asking for the sample
  auto sp_to_node = net[*it_topo];
  //Checking if the node is Evidence
  if(sp_to_node->IsEvidence() == true){
   //unsigned int node_evidence = sp_to_node->GetEvidence();
   std::pair<bool,unsigned int> pair_to_store = std::make_pair<bool, unsigned int>(true, sp_to_node->GetEvidence());
   sample_map[*it_topo] = pair_to_store;
   weight_to_return = weight_to_return * sp_to_node->conditionalTable.GetProbability(sp_to_node->GetEvidence(), key_vector);
  }else{
   //If it is not an Evidence then storing the sample in the local index vector
   std::pair<bool,unsigned int> pair_to_store = std::make_pair<bool, unsigned int>(true, sp_to_node->conditionalTable.ReturnSample(key_vector));
   sample_map[*it_topo] = pair_to_store;
  }
 }

 //Fill the vector_to_return with the result stored inside the map
 //This cycle restore the nodes order (not the topological order)
 for(unsigned int i=0; i<tot_nodes; i++){
  auto temp_pair = sample_map[i];
  vector_to_return[i] = temp_pair.second; //Storing the sample into the vector to return
 }

 return std::make_pair(vector_to_return, weight_to_return);
}

/**
* It accumulate samples picking up them from the Bayesian network.
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
*
**/
std::vector<std::pair<std::vector<unsigned int>, double>> LWSampler::AccumulateSamples(Bayesnet& net, unsigned int cycles){
 std::vector<std::pair<std::vector<unsigned int>, double>> vector_to_return;
 for(unsigned int i=0; i<cycles; i++){
  vector_to_return.push_back(ReturnSample(net));
 }
 return vector_to_return;
}


/**
* It prints the result of the sampling.
* It is possible to do it for different iterations.
* The format used is the following :
* INDEX ..... SAMPLE ..... WEIGHT
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
*
**/
void LWSampler::PrintSample(bayonet::Bayesnet& net, unsigned int cycles){
 for(unsigned int i=0; i<cycles; i++){
  auto sample_pair = ReturnSample(net);
  auto sample_vector = sample_pair.first;
  auto weight = sample_pair.second;
  std::cout << i+1 << " ..... ";
  for(auto it=sample_vector.begin(); it!=sample_vector.end(); ++it){
   std::cout << *it << " ";
  }
  std::cout << " ..... " << weight << std::endl;
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
JointProbabilityTable LWSampler::ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles){

 //0-Declare the JPT
 JointProbabilityTable joint_table(net.ReturnTotalStates());

 //1-reset JPT
 joint_table.ResetProbabilities();

 
 //auto samples_vector = AccumulateSamples(net, cycles);

 //2-Accumulate samples and Add sample and weight to JPT
 for(unsigned int i=0; i<cycles; i++){
  auto sample_pair = ReturnSample(net);
  joint_table.AddToProbability(sample_pair.first, sample_pair.second);
 }

 //3-Add sample and weight to JPT
 //for(auto it_sample=samples_vector.begin(); it_sample!=samples_vector.end(); ++it_sample){
  //joint_table.AddToProbability(it_sample->first, it_sample->second);
 //}

 //4-Normalize JPT
 joint_table.NormalizeProbabilities();

 //5-Return JPT
 return joint_table;
}



} //namespace



