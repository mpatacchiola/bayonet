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

#include"GibbsSampler.h"
#include <iostream>
#include <random>
#include <list>
#include <utility>      // std::pair
#include <ctime>

namespace bayonet{


GibbsSampler::GibbsSampler(){}

GibbsSampler::~GibbsSampler(){}

/**
* It returns a single sample picking up it from the Bayesian network
*
* @param net the Bayesian network to use for picking up the sample.
*
**/
std::vector<unsigned int> GibbsSampler::ReturnSample(bayonet::Bayesnet& net){

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
    std::cerr << "EXCEPTION: the topological order was not respected!" << std::endl;   
   }else{
    key_vector.push_back(value_sample); //push   
   }
  }
  //Key completed, asking for the sample
  auto sp_to_node = net[*it_topo];
  //Checking if the node is Evidence
  if(sp_to_node.IsEvidence() == true){
   //unsigned int node_evidence = sp_to_node->GetEvidence();
   std::pair<bool,unsigned int> pair_to_store = std::make_pair<bool, unsigned int>(true, sp_to_node.GetEvidence());
   sample_map[*it_topo] = pair_to_store;
  }else{
   //If it is not an Evidence then storing the sample in the local index vector
   std::pair<bool,unsigned int> pair_to_store = std::make_pair<bool, unsigned int>(true, sp_to_node.conditionalTable.ReturnSample(key_vector));
   sample_map[*it_topo] = pair_to_store;
  }

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
* It returns a single sample picking up it from the Bayesian network
*
* @param net the Bayesian network to use for picking up the sample.
* @param startingVector it is the vector with the current states of all the nodes
* it is used as starting point for the Markov chain sampler
**/
std::vector<unsigned int> GibbsSampler::ReturnSample(bayonet::Bayesnet& net, std::vector<unsigned int> startingVector){
std::vector<unsigned int> vector_to_return;

 //Return list of not-evidence nodes
 auto not_evidence_nodes_list = net.ReturnNotEvidenceNodes();

 //Checking for empty Networks
 if(net.ReturnNumberOfNodes() == 0) return vector_to_return; //The net is empty

 //Check if the network contains only evidence nodes
 if(not_evidence_nodes_list.size() == 0){
  return startingVector;
 }

  unsigned int temp_int = ReturnInteger(0, not_evidence_nodes_list.size()-1);
  unsigned int selected_node = not_evidence_nodes_list[temp_int]; //ReturnRandomFromList(not_evidence_nodes);
  auto sp_to_node = net[selected_node];

  //It is necessary to iterate the different states of the node
  //in order to build the final conditional distribution
  std::vector<unsigned int> temp_sample_vector = startingVector;
  unsigned int node_tot_states = sp_to_node.ReturnNumberOfStates();
  std::vector<double> conditional_distribution_vector;
  for(unsigned int i_states=0; i_states<node_tot_states; i_states++){

	  temp_sample_vector[selected_node] = i_states;
	  //1- The first phase consists in finding the probability
	  //of the selected node given its parents
	  auto in_list = net.ReturnInEdges(selected_node);
	  std::vector<unsigned int> key_vector;
	  //Cycle through the in list for creating the key
	  for(auto it_in=in_list.begin(); it_in!=in_list.end(); ++it_in){
	   unsigned int selected_state = startingVector[*it_in]; //take the state of the parents of the node
	    key_vector.push_back(selected_state); //push the state in the key vector  
	  }
	  //Key completed, asking for the probability
	  double parents_prob = sp_to_node.conditionalTable.GetProbability(i_states ,key_vector);

	  //2- The second phase consists in finding the probability
	  //for each children of the current node, given the parents
	  auto out_list = net.ReturnOutEdges(selected_node);
	  std::vector<double> children_prob_vector;
	  for(auto it_out=out_list.begin(); it_out!=out_list.end(); ++it_out){ 
	   double returned_prob = net.GetNodeProbability(*it_out, temp_sample_vector);
	   children_prob_vector.push_back(returned_prob);
	  }

	  //3- The last phase is the multiplication of the different
	  //probabilities in order to get the final Markov blanket conditional probability
	  double conditional_prob = parents_prob;
	  for(auto it_children=children_prob_vector.begin(); it_children!=children_prob_vector.end(); ++it_children){ 
	   conditional_prob *= *it_children;
	  }
	conditional_distribution_vector.push_back(conditional_prob);
  }

 //4- It is necessary to normalize the conditional distribution before sampling
 double accumulator = 0.0;
 for(auto it_dist=conditional_distribution_vector.begin(); it_dist!=conditional_distribution_vector.end(); ++it_dist){
  accumulator += *it_dist;
 }
 for(auto it_dist=conditional_distribution_vector.begin(); it_dist!=conditional_distribution_vector.end(); ++it_dist){
  *it_dist = *it_dist / accumulator;
 }

 //5- It is time to sample from the distribution
 std::random_device random_device;
 std::mt19937 generator(random_device());
 std::discrete_distribution<unsigned int> values_distribution (conditional_distribution_vector.begin(), conditional_distribution_vector.end());
 unsigned int node_sample =values_distribution(generator);
 startingVector[selected_node] = node_sample;

 //6- Returning the modified sample vector
 return startingVector;
}


/*
std::vector<std::vector<unsigned int>> GibbsSampler::AccumulateSamples(Bayesnet& net, unsigned int cycles){
 std::vector<std::vector<unsigned int>> vector_to_return;

 //The first sample is taken at random
 auto starting_sample_vector = ReturnSample(net);
 vector_to_return.push_back(starting_sample_vector);

 //Return list of not-evidence nodes
 auto not_evidence_nodes_list = net.ReturnNotEvidenceNodes();

 //Checking for empty Networks
 if(net.ReturnNumberOfNodes() == 0) return vector_to_return; //The net is empty

 //Check if the network contains only evidence nodes
 if(not_evidence_nodes_list.size() == 0){
  for(unsigned int i=0; i<cycles-1; i++){
   vector_to_return.push_back(starting_sample_vector);
  }
  return vector_to_return;
 }

 //The starting_sample is modified one variable at time
 //and the result is stored. The vairable to change is
 //choosen at random as well.
 for(unsigned int i=0; i<cycles-1; i++){

  unsigned int temp_int = ReturnInteger(0, not_evidence_nodes_list.size()-1);
  unsigned int selected_node = not_evidence_nodes_list[temp_int]; //ReturnRandomFromList(not_evidence_nodes);
  auto sp_to_node = net[selected_node];

  //It is necessary to iterate the different states of the node
  //in order to build the final conditional distribution
  std::vector<unsigned int> temp_sample_vector = starting_sample_vector;
  unsigned int node_tot_states = sp_to_node->ReturnNumberOfStates();
  std::vector<double> conditional_distribution_vector;
  for(unsigned int i_states=0; i_states<node_tot_states; i_states++){

	  temp_sample_vector[selected_node] = i_states;
	  //1- The first phase consists in finding the probability
	  //of the selected node given its parents
	  auto in_list = net.ReturnInEdges(selected_node);
	  std::vector<unsigned int> key_vector;
	  //Cycle through the in list for creating the key
	  for(auto it_in=in_list.begin(); it_in!=in_list.end(); ++it_in){
	   unsigned int selected_state = starting_sample_vector[*it_in]; //take the state of the parents of the node
	    key_vector.push_back(selected_state); //push the state in the key vector  
	  }
	  //Key completed, asking for the sample
	  double parents_prob = sp_to_node->conditionalTable.GetProbability(i_states ,key_vector);
	  //Storing the sample in the local index vector
	  //unsigned int the_sample = sp_to_node->conditionalTable.ReturnSample(key_vector);
	  //starting_sample_vector[selected_node] = the_sample;
	  //vector_to_return.push_back(starting_sample_vector);

	  //2- The second phase consists in finding the probability
	  //for each children of the current node, given the parents
	  auto out_list = net.ReturnOutEdges(selected_node);
	  std::vector<double> children_prob_vector;
	  for(auto it_out=out_list.begin(); it_out!=out_list.end(); ++it_out){ 
	   double returned_prob = net.GetNodeProbability(*it_out, temp_sample_vector);
	   children_prob_vector.push_back(returned_prob);
	  }

	  //3- The last phase is the multiplication of the different
	  //probabilities in order to get the final Markov blanket conditional probability
	  double conditional_prob = parents_prob;
	  for(auto it_children=children_prob_vector.begin(); it_children!=children_prob_vector.end(); ++it_children){ 
	   conditional_prob *= *it_children;
	  }
	conditional_distribution_vector.push_back(conditional_prob);
  }

 //4- It is necessary to normalize the conditional distribution before sampling
 double accumulator = 0.0;
 for(auto it_dist=conditional_distribution_vector.begin(); it_dist!=conditional_distribution_vector.end(); ++it_dist){
  accumulator += *it_dist;
 }
 for(auto it_dist=conditional_distribution_vector.begin(); it_dist!=conditional_distribution_vector.end(); ++it_dist){
  *it_dist = *it_dist / accumulator;
 }

 //5- It is time to sample from the distribution
 std::random_device random_device;
 std::mt19937 generator(random_device());
 std::discrete_distribution<unsigned int> values_distribution (conditional_distribution_vector.begin(), conditional_distribution_vector.end());
 unsigned int node_sample =values_distribution(generator);
 starting_sample_vector[selected_node] = node_sample;

 //6- Pushing inside the output vector the new sample
 vector_to_return.push_back(starting_sample_vector);

 }
 return vector_to_return;
}

*/

/**
* This method is different from the same methods in the other samplers.
* The first sample is obtained at random. The next samples are choosen
* for each node picking up a value from the Markov blanket of the node.
* This probability is proportional to the probability of the variable
* given its parents times the probability of each child given its 
* respective parents. 
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
*
**/
std::vector<std::vector<unsigned int>> GibbsSampler::AccumulateSamples(Bayesnet& net, unsigned int cycles){
 std::vector<std::vector<unsigned int>> accumulated_samples_vector; 
 std::vector<unsigned int> starting_sample_vector = ReturnSample(net);
 
 for(unsigned int i=0; i<cycles; i++){
  accumulated_samples_vector.push_back(starting_sample_vector);
  starting_sample_vector =  ReturnSample(net, starting_sample_vector);
 }
 return accumulated_samples_vector;
}

/**
* It prints the result of the sampling.
* It is possible to do it for different iterations.
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
*
**/
void GibbsSampler::PrintSample(bayonet::Bayesnet& net, unsigned int cycles){
 std::vector<unsigned int> starting_sample_vector = ReturnSample(net);
 
 for(unsigned int i=0; i<cycles; i++){
  std::cout << i+1 << " ..... ";
  for(auto it=starting_sample_vector.begin(); it!=starting_sample_vector.end(); ++it){
   std::cout << *it << " ";
  }
  std::cout << std::endl;
  starting_sample_vector =  ReturnSample(net, starting_sample_vector);
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
JointProbabilityTable GibbsSampler::ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles){

 //0-Declare the JPT
 JointProbabilityTable joint_table(net.ReturnTotalStates());

 //1-reset JPT
 joint_table.ResetProbabilities();

 //2-Accumulate samples
 auto samples_vector = AccumulateSamples(net, cycles);

 //3-Add sample to JPT
 for(auto it_sample=samples_vector.begin(); it_sample!=samples_vector.end(); ++it_sample){
  joint_table.AddToProbability(*it_sample, 1);
 }

 //4-Normalize JPT
 joint_table.NormalizeProbabilities();

 //5-Return JPT
 return joint_table;
}


/**
 * Return a random generated integer.
 * It is used a uniform distribution.
 * @param minRange it is the minimum value to use for selecting the integer
 * @param maxRange it is the maximum value to use for selecting the integer
 * @return It returns the selected integer
*/
int GibbsSampler::ReturnInteger(const int minRange, const int maxRange) {
    std::srand (std::time(NULL));
    std::random_device generator_device;
    std::mt19937_64 gen(generator_device());
    std::uniform_int_distribution<int> distribution_device(minRange,maxRange);
    return distribution_device(gen);
}


} //namespace



