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


//This example is based on the implementation of the paper:
//"A developmental Bayesian model of trust in artificial cognitive systems".
//(M. Patacchiola, A. Cangelosi)
//Using the same approach a different experiment is reproduced: 
//"Preschoolers mistrust ignorant and inaccurate speakers" 
//(M. A. Koenig and P. L. Harris, Child Development, vol. 76)
//
//"A developmental Bayesian model of trust in artificial cognitive systems".
//Abstract: In human-robot interaction trust is one of the main
//factors to take into account for enabling effective interaction.
//Limited models exist that delineate the development of trust in
//real world scenarios. Reshaping one of these models we show how
//a probabilistic framework based on Bayesian Networks (BNs)
//can incorporate the reliability of information sources into the
//decisional process of artificial systems. Furthermore, using a
//developmental approach we gain some insight on how children
//estimate people’s reliability and how some aspects of the Theory
//of Mind (ToM) can affect that estimation. To test the model
//we reproduced a developmental experiment in a computational
//simulation and we embedded the BNs inside an artificial agent.
//The simulation results are in line with the real data, and confirm
//that BNs have the potential for being included as trust evaluator
//modules in robotic systems.
//
//Article: "Preschoolers Mistrust Ignorant and Inaccurate Speakers"
//Being able to evaluate the accuracy of an informant is essential to 
//communication. Three experiments explored preschoolers' (N=119) 
//understanding that, in cases of conflict, information from reliable 
//informants is preferable to information from unreliable informants. 
//In Experiment 1, children were presented with previously accurate 
//and inaccurate informants who presented conflicting names for novel 
//objects. 4-year-olds—but not 3-year-olds—predicted whether an informant 
//would be accurate in the future, sought, and endorsed information 
//from the accurate over the inaccurate informant. In Experiment 2, both 
//age groups displayed trust in knowledgeable over ignorant speakers. 
//In Experiment 3, children extended selective trust when learning both 
//verbal and nonverbal information. These experiments demonstrate that 
//preschoolers have a key strategy for assessing the reliability of 
//information.
//
//The files with extension .tra are training files and they can be used
//to estimate the parameters of the BN. Each row of the file must contain
//the state value of each node for a single interaction:
//e.g. 0,1,0,1,0,0 means that the first node has state 0, the second 1
//the third 0, the fourth 1, etc
//This states are Bayesian "evidence" which must be used by the 
//Maximum Likelihood Estimator.


#include<bayonet/Bayesnet.h>
#include<bayonet/Bayesnode.h>
#include<bayonet/ConditionalProbabilityTable.h>
#include<bayonet/JointProbabilityTable.h>
#include<bayonet/RejectionSampler.h>
#include<bayonet/GibbsSampler.h>
#include<bayonet/BeliefPropagator.h>
#include<bayonet/MaximumLikelihoodLearning.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>


//This function allows loading the .tra file (which is a CSV file)
std::vector<std::vector<unsigned int>> CreateDatasetFromFile(std::string filePath){
 std::vector<std::vector<unsigned int>> dataset_vector;
 std::ifstream train_file(filePath);
 std::string token;

 while(std::getline(train_file, token)) {
  token.erase(std::remove_if(token.begin(), token.end(), isspace), token.end());
  std::vector<unsigned int> vect;
  std::stringstream ss(token);
  unsigned int i;
  while(ss >> i){
   vect.push_back(i);
   if(ss.peek()== ',') ss.ignore();
  }

  dataset_vector.push_back(vect);
 }
 return dataset_vector;
}



int main()
{


 enum state_none{
  BALL=0,
  CUP=1,
  NONE=2
 };

 enum node{
  CHILD_BELIEF=0,
  CHILD_STATE=1,
  R_BELIEF=2,
  R_STATE=3,
  U_BELIEF=4,
  U_STATE=5
 };

 enum state{
  A=0,
  B=1,
  C=2
 };

 //TRAIN DATASET
 std::vector<std::vector<unsigned int>> dataset_vector;
 //change the name of the file based on the situation you want to reproduce
 std::string train_path = "./sim1_helper_it.tra";
 dataset_vector = CreateDatasetFromFile(train_path);

 unsigned int iterations = 100000; //It defines the number of iterations used in the sampler
 bayonet::Bayesnet myNet({3, 3, 3, 3, 3, 3}); //It creates a net with X nodes, each node can assume 3 states
 bayonet::GibbsSampler myGibbs; //It defines the rejection sampler
 bayonet::BeliefPropagator myPropagator;
 myNet.AddEdge(CHILD_BELIEF, CHILD_STATE);
 myNet.AddEdge(R_BELIEF, R_STATE);
 myNet.AddEdge(U_BELIEF, U_STATE);
 myNet.AddEdge(R_STATE, CHILD_STATE);
 myNet.AddEdge(U_STATE, CHILD_STATE);

 std::cout << std::endl << "=============== BEFORE LEARNING ===============" << std::endl;
 myPropagator.InitialTree(myNet);
 myPropagator.UpdateTree(myNet);
 bayonet::JointProbabilityTable myJointTable = myPropagator.ReturnJointProbabilityTable(myNet);
 //bayonet::JointProbabilityTable myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 std::cout << std::endl << "------------- CHILD_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_BELIEF);
 std::cout << std::endl << "------------- CHILD_STATE-------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_STATE);
 std::cout << std::endl << "------------- R_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(R_BELIEF);
 std::cout << std::endl << "------------- R_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(R_STATE);
 std::cout << std::endl << "------------- U_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(U_BELIEF);
 std::cout << std::endl << "------------- U_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(U_STATE);
 std::cout << std::endl;


 //Learning phase, here the dataset obtained through the
 // training file is used to adjust the network parameters
 bayonet::MaximumLikelihoodLearning myLearner;
 myNet = myLearner.ReturnUpdatedNetwork(myNet, dataset_vector);

 //Printing utilities (uncomment for debugging)
 //myNet[0].conditionalTable.Print();
 // myNet[1].conditionalTable.Print();
 //myNet[2].conditionalTable.Print();
 //myNet[3].conditionalTable.Print();
 //myNet[4].conditionalTable.Print();
 //myNet[5].conditionalTable.Print();

 //myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 //myJointTable.Print();


 std::cout << std::endl << "=============== *JUDGMENT TRIAL ===============" << std::endl;
 std::cout << std::endl << "Which people was not very good at answering questions?" << std::endl;

 //PROPAGATOR
 //myPropagator.InitialTree(myNet);
 //myPropagator.UpdateTree(myNet);
 //myJointTable = myPropagator.ReturnJointProbabilityTable(myNet);
 myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 std::cout << std::endl << "------------- CHILD_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_BELIEF);
 std::cout << std::endl << "------------- CHILD_STATE-------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_STATE);
 std::cout << std::endl << "------------- R_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(R_BELIEF);
 std::cout << std::endl << "------------- R_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(R_STATE);
 std::cout << std::endl << "------------- U_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(U_BELIEF);
 std::cout << std::endl << "------------- U_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(U_STATE);
 std::cout << std::endl;


 std::cout << std::endl << "=============== PREDICTION TRIAL ===============" << std::endl;
 std::cout << std::endl << "Familiar object presented. Which name are gonna think the people?" << std::endl;
 //EVIDENCE
 //myNet[RELIABLE].SetEvidence(true);
 //myNet[UNRELIABLE].SetEvidence(false);
 myNet[CHILD_BELIEF ].SetEvidence(A);
 myNet[CHILD_STATE].SetEvidence(A);

 //PROPAGATOR
 //myPropagator.InitialTree(myNet);
 //myPropagator.UpdateTree(myNet);
 //myJointTable = myPropagator.ReturnJointProbabilityTable(myNet);
 myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 std::cout << std::endl << "------------- CHILD_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_BELIEF);
 std::cout << std::endl << "------------- CHILD_STATE-------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_STATE);
 std::cout << std::endl << "------------- R_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(R_BELIEF);
 std::cout << std::endl << "------------- R_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(R_STATE);
 std::cout << std::endl << "------------- U_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(U_BELIEF);
 std::cout << std::endl << "------------- U_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(U_STATE);
 std::cout << std::endl;

 std::cout << std::endl << "=============== *ASK TRIAL ===============" << std::endl;
 std::cout << std::endl << "Novel Object presented. Which people do you wanna ask for help?" << std::endl;
 //EVIDENCE
 myNet.ResetAllEvidences();
 myNet[CHILD_BELIEF].SetEvidence(A);
 myNet[CHILD_STATE].SetEvidence(A);
 //PROPAGATOR
 //myPropagator.InitialTree(myNet);
 //myPropagator.UpdateTree(myNet);
 //myJointTable = myPropagator.ReturnJointProbabilityTable(myNet);
 myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 std::cout << std::endl << "------------- CHILD_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_BELIEF);
 std::cout << std::endl << "------------- CHILD_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_STATE);
 std::cout << std::endl << "------------- R_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(R_BELIEF);
 std::cout << std::endl << "------------- R_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(R_STATE);
 std::cout << std::endl << "------------- U_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(U_BELIEF);
 std::cout << std::endl << "------------- U_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(U_STATE);
 std::cout << std::endl;

 std::cout << std::endl << "=============== ENDORSEMENT TRIAL ===============" << std::endl;
 std::cout << std::endl << "The reliable teacher says it is a Mido, the unreliable says it is a Ioma. What's the name of the object?" <<   std::endl;
 //EVIDENCE
 myNet.ResetAllEvidences();
 myNet[R_STATE].SetEvidence(A);
 myNet[U_STATE].SetEvidence(C);

 //PROPAGATOR
 //myPropagator.InitialTree(myNet);
 //myPropagator.UpdateTree(myNet);
 //myJointTable = myPropagator.ReturnJointProbabilityTable(myNet);
 myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 std::cout << std::endl << "------------- CHILD_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_BELIEF);
 std::cout << std::endl << "------------- CHILD_STATE-------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_STATE);
 std::cout << std::endl << "------------- R_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(R_BELIEF);
 std::cout << std::endl << "------------- R_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(R_STATE);
 std::cout << std::endl << "------------- U_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(U_BELIEF);
 std::cout << std::endl << "------------- U_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(U_STATE);
 std::cout << std::endl;


 std::cout << std::endl << "=============== HALF ENDORSEMENT TRIAL (R) ===============" << std::endl;
 std::cout << std::endl << "The reliable teacher says it is a Mido. What's the name of the object?" << std::endl;
 //EVIDENCE
 myNet.ResetAllEvidences();
 myNet[R_STATE].SetEvidence(A);
 //myNet[U_STATE].SetEvidence(C);

 //PROPAGATOR
 //myPropagator.InitialTree(myNet);
 //myPropagator.UpdateTree(myNet);
 //myJointTable = myPropagator.ReturnJointProbabilityTable(myNet);
 myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 std::cout << std::endl << "------------- CHILD_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_BELIEF);
 std::cout << std::endl << "------------- CHILD_STATE-------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_STATE);
 std::cout << std::endl << "------------- R_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(R_BELIEF);
 std::cout << std::endl << "------------- R_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(R_STATE);
 std::cout << std::endl << "------------- U_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(U_BELIEF);
 std::cout << std::endl << "------------- U_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(U_STATE);
 std::cout << std::endl;


 std::cout << std::endl << "=============== HALF ENDORSEMENT TRIAL (U) ===============" << std::endl;
 std::cout << std::endl << "The unreliable says it is a Ioma. What's the name of the object?" << std::endl;
 //EVIDENCE
 myNet.ResetAllEvidences();
 //myNet[R_STATE].SetEvidence(A);
 myNet[U_STATE].SetEvidence(C);

 //PROPAGATOR
 //myPropagator.InitialTree(myNet);
 //myPropagator.UpdateTree(myNet);
 //myJointTable = myPropagator.ReturnJointProbabilityTable(myNet);
 myJointTable = myGibbs.ReturnJointProbabilityTable(myNet, iterations);
 std::cout << std::endl << "------------- CHILD_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_BELIEF);
 std::cout << std::endl << "------------- CHILD_STATE-------------" << std::endl;
 myJointTable.PrintMarginal(CHILD_STATE);
 std::cout << std::endl << "------------- R_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(R_BELIEF);
 std::cout << std::endl << "------------- R_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(R_STATE);
 std::cout << std::endl << "------------- U_BELIEF -------------" << std::endl;
 myJointTable.PrintMarginal(U_BELIEF);
 std::cout << std::endl << "------------- U_STATE -------------" << std::endl;
 myJointTable.PrintMarginal(U_STATE);
 std::cout << std::endl;


 return 0;
}
