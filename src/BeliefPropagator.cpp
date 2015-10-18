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

#include"BeliefPropagator.h"
#include <iostream>
#include <random>
#include <list>
#include <utility>      // std::pair

namespace bayonet{


BeliefPropagator::BeliefPropagator(){}

BeliefPropagator::~BeliefPropagator(){}

/**
* It creates a Joint Probability table starting from the Bayesian network and sampling for
* the number of iterations specified.
*
* @param net the Bayesian network to use for picking up the sample.
* @param cycles the number of iterations
* @return it return a Joint Probability Table object
**/
JointProbabilityTable BeliefPropagator::ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles){

 //0-Declare the JPT
 JointProbabilityTable joint_table(net.ReturnTotalStates());


 //5-Return JPT
 return joint_table;
}


/**
* It initializes the network tree.
*
* @param net the bayesian network to initialize
**/
void BeliefPropagator::InitialTree(Bayesnet& net){

 //1- It clear the vector, to prevent problem
 //if the method is called more than once.
 parametersVector.clear();

 //2- It fills the parametersVector with the right number of parameters
 for(unsigned int i_net=0; i_net<net.ReturnNumberOfNodes(); i_net++){
  unsigned int node_tot_states = net[i_net].ReturnNumberOfStates();
  std::vector<parameters> temp_vector;
  for(unsigned int i_states=0; i_states<node_tot_states; i_states++){
   parameters temp_parameters;
   temp_parameters.belief=0;
   temp_parameters.pi_value=0;
   temp_parameters.lambda_value=0;
   temp_vector.push_back(temp_parameters);   
  } 
  parametersVector.push_back(temp_vector);
 }

 //3- Iteration through all the nodes
 unsigned int nodes_counter = 0;
 for(auto it_node=parametersVector.begin(); it_node!=parametersVector.end(); ++it_node){
  //Iteration through the states-parameters
  unsigned int states_counter = 0;

  for(auto it_state=it_node->begin(); it_state!=it_node->end(); ++it_state){  
   //if not root node, it initializes at 1 everything
   if(net.IsRoot(nodes_counter) == false){
    it_state->pi_value = 1;
    it_state->lambda_value = 1;
   //If root node, then pi_values are equal to the associated conditional probability
   }else{
    it_state->pi_value = net[nodes_counter].conditionalTable.GetProbability(states_counter, {});
    it_state->lambda_value = 1;
   }

   //Check if evidence node and if current state is evidence
   if(net[nodes_counter].IsEvidence() == true && net[nodes_counter].GetEvidence()==states_counter){
    it_state->pi_value = 1;
    it_state->lambda_value = 1;
   //If root node, then pi_values are equal to the associated conditional probability
   }

   if(net[nodes_counter].IsEvidence() == true && net[nodes_counter].GetEvidence()!=states_counter){
    it_state->pi_value = 0;
    it_state->lambda_value = 0;
   //If root node, then pi_values are equal to the associated conditional probability
   }

   states_counter++;
  }  
  nodes_counter++;
 }
}

/**
* It updates the Belief for all the nodes in the network.
* The Tree must be initialized before calling this function
*
* @param net The network to Update
**/
void BeliefPropagator::UpdateTree(Bayesnet& net){
 
 unsigned int nodes_counter = 0;
 for(auto it_nodes=parametersVector.begin(); it_nodes!=parametersVector.end(); ++it_nodes){
  unsigned int states_counter = 0;
  double alfa = 0.0; //normalizing constant, used at the end for normalizing the belief of the node
  for(auto it_states=it_nodes->begin(); it_states!=it_nodes->end(); ++it_states){

  //0-Before any operation of updating it is necessary to reset everything
  //it_states->lambda_value = 1;  //to 1 because it is multiplied
  it_states->pi_value = 0; //to 0 because it is added

   //1-Calculating lambda_values
   if(net[nodes_counter].IsEvidence() == true && net[nodes_counter].GetEvidence()==states_counter) it_states->lambda_value=1.0;
   if(net[nodes_counter].IsEvidence() == true && net[nodes_counter].GetEvidence()!=states_counter) it_states->lambda_value=0.0;
   if(net[nodes_counter].IsEvidence() == false){
    auto out_list=net.ReturnOutEdges(nodes_counter);
    for(auto it_out=out_list.begin(); it_out!=out_list.end(); ++it_out){
     //Asking lambda_messages to all the children
    //Y send X a lambda_message
   //(net, unsigned int Y, unsigned int X, unsigned int X_state)
      double local_lambda_message = ReturnLambdaMessage(net, *it_out, nodes_counter, states_counter);
      it_states->lambda_value *= local_lambda_message;
    }
   }

   //2-Calculating pi_values
   unsigned int cpt_tot_rows = net[nodes_counter].conditionalTable.ReturnRowsNumber();
   std::vector<unsigned int> parents_vector;
   double state_probability = 0.0;
   for(unsigned int i_row=0; i_row<cpt_tot_rows; i_row++){
    parents_vector = net[nodes_counter].conditionalTable.ReturnRow(i_row).first;
    state_probability = net[nodes_counter].conditionalTable.GetProbability(states_counter, parents_vector);
    double temp_multiplier = state_probability;
    auto it_in_list = net.ReturnInEdges(nodes_counter);
    unsigned int parent_counter = 0;
    for(auto it_in=it_in_list.begin(); it_in!=it_in_list.end(); ++it_in){
     //Asking pi_messages to all the parents
        double local_pi_message = ReturnPiMessage(net, *it_in, parents_vector[parent_counter], nodes_counter);
        temp_multiplier *= local_pi_message;
	parent_counter++;
    }
    it_states->pi_value += temp_multiplier;
   }

   //3-Calculating Belief
   if(net[nodes_counter].IsEvidence() == true && net[nodes_counter].GetEvidence()!=states_counter) it_states->pi_value=0;
   it_states->belief = it_states->pi_value * it_states->lambda_value;
   alfa += it_states->belief; //accumulating the normalization constant
   states_counter++;
  }

  //4-Normalizing the belief of the node
  for(auto it_states=it_nodes->begin(); it_states!=it_nodes->end(); ++it_states){
    it_states->belief = it_states->belief / alfa;
  }

  nodes_counter++;
 }

}


/**
* It return a vector of pi messages from a node
* Z send X a pi_message
* Z -> X -> Y
**/
double BeliefPropagator::ReturnPiMessage(bayonet::Bayesnet& net, unsigned int Z, unsigned int Z_state, unsigned int X){

 //if(net.IsRoot(Z) == true) return parametersVector[Z][Z_state].pi_value;

 //PI_MESS from Z to X2 = PI_VAL(Z) * LAMBDA_MESS(X1 to Z) * LAMBDA_MESS(X3 to Z)
 //auto z_param_vector = parametersVector[Z];
 double z_pi_value = parametersVector[Z][Z_state].pi_value;

 //0- Check if X is a child of Z, and its position in the in_list of  Z
 auto z_out_list = net.ReturnInEdges(Z);
 //unsigned int position_counter=0;

 //Iterate through Z out_list (children) asking for lambda messages
 double lambda_mess_multiplier = 1;
 for(auto it_out=z_out_list.begin(); it_out!=z_out_list.end(); ++it_out){
  if(*it_out != X) lambda_mess_multiplier *= ReturnLambdaMessage(net, *it_out, Z, Z_state); //all children send to Z the lambda_mess
 }

 return z_pi_value * lambda_mess_multiplier;
}

/**
* It return a vector of lambda messages from a node
* Y send X a Lambda_message
* Z -> X -> Y
**/
double BeliefPropagator::ReturnLambdaMessage(bayonet::Bayesnet& net, unsigned int Y, unsigned int X, unsigned int X_state){
 //auto y_param_vector = parametersVector[Y];
 double final_result = 0;

 //0- Check if X is a parent of Y, and its position in the in_list of  Y
 auto y_in_list = net.ReturnInEdges(Y);
 unsigned int position_counter=0;
 unsigned int pos_x_in_y = 0;
 for(auto it_in=y_in_list.begin(); it_in!=y_in_list.end(); ++it_in){
  if(*it_in == X) pos_x_in_y = position_counter;
  position_counter++;
 }//Well now we have the position of X in Y

 //1- It get a list of index, representing the position of X_state in Y CPT
 std::vector<unsigned int> parent_position_vector;
 parent_position_vector = net[Y].conditionalTable.FindParentState(pos_x_in_y, X_state);

 //2- Using the list at the previous step it iterates and returns the parent states vector of Y
 //The parent states vector is used for asking specific PI_MESS necessary to compute the LAMBDA_MESS
 for(unsigned int i_param=0; i_param<parametersVector[Y].size(); i_param++){
  double temp_result = 0;

  for(auto it_par=parent_position_vector.begin(); it_par!=parent_position_vector.end(); ++it_par){
   auto temp_pair = net[Y].conditionalTable.ReturnRow(*it_par);
   auto parents_states_vector = temp_pair.first;
   double probability = temp_pair.second[i_param];
   double pi_mess_multiplier = 1;
   //For all the in nodes (but the X node) we ask for the pi_messages
   //and we multiply the values in a variable
   unsigned int i_parent=0;
   for(auto it_in=y_in_list.begin(); it_in!=y_in_list.end(); ++it_in){
    if(i_parent!=pos_x_in_y){
     //Here Y ask to all its parents (but X) to send pi_messages
     double local_pi_message = ReturnPiMessage( net, *it_in, parents_states_vector[i_parent], Y);
     pi_mess_multiplier *= local_pi_message;
    }
    i_parent++;
   }

   temp_result +=  probability * pi_mess_multiplier; //Adding the PROBABILITY and PI_MESS to temp result
  } 
  temp_result *= parametersVector[Y][i_param].lambda_value; //Multiplying the temp result times LAMBDA_VALUE
  final_result +=  temp_result; //Adding to final result
 }

 return final_result;
}


void BeliefPropagator::Print(){
 unsigned int nodes_counter = 0;
 for(auto it_first= parametersVector.begin(); it_first!= parametersVector.end(); ++it_first){
   std::cout << "NODE: " << nodes_counter << std::endl;
   for(auto it_states=it_first->begin(); it_states!=it_first->end(); ++it_states){
     std::cout << "belief: " << it_states->belief;
     std::cout << " pi_value: " << it_states->pi_value;
     std::cout << " lambda_value: " << it_states->lambda_value;
     std::cout << std::endl;
   }
    std::cout << std::endl;

  nodes_counter++;
 }

}


} //namespace



