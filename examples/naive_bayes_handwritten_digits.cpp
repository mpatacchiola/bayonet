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

/* The naive Bayes classifier is one of the simplest form of Bayesian
 * network. There is a main parent node and an unspecified number
 * of children nodes. The "naive" assumption is the independence 
 * between every pair of features. The naive classifier is an over
 * simplified classifier but it works well in different real world
 * application (for example spam-filtering).
 *
 * In this example I will use an handwritten digits dataset to
 * learn a naive Bayesian network using the Maximum Likelihood
 * Estimator. The structure of the network derives from the structure
 * of the dataset. Each line of the dataset is a 17-dimensional
 * vector, where the integers in the range 0-15 represent the pixel 
 * intensity (from 0 to 100) of a 4x4 matrix. The last number
 * of the line represent the coded digit. The corresponding 
 * network has a main node with 10 states (the digits from 0 to 9) 
 * and 15 children node with 101 states (the pixel intensity 
 * from 0 to 100).
 *
 *
 *           [DIGITS]
 *           // ||  \\
 *          //  ||   \\
 *         //   ||    \\
 *        //    ||     \\
 *       \/     \/      \/
 *     [P00]   ....    [P33]
 *
 *
 * The dataset is included in the examples folder, and it is
 * uncompressed during the installation. The references to
 * the dataset are reported below.
 *
 *
 * Link to the dataset:
 * https://archive.ics.uci.edu/ml/datasets/Pen-Based+Recognition+of+Handwritten+Digits
 *
 * Dataset Source:
 *
 * E. Alpaydin, Fevzi. Alimoglu 
 * Department of Computer Engineering 
 * Bogazici University, 80815 Istanbul Turkey 
 * alpaydin '@' boun.edu.tr
 * 
 * 
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <math.h>  //round

//Including Bayonet libraries
#include<bayonet/Bayesnet.h>
#include<bayonet/Bayesnode.h>
#include<bayonet/MarginalProbabilityTable.h>
#include<bayonet/LWSampler.h>
#include<bayonet/BeliefPropagator.h>
#include<bayonet/MaximumLikelihoodLearning.h>

//This function create a vector of vector from
//the dataset text file. 
//
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



int main(){

 //An enum structure permits to make the nodes
 //more readable. P00 is the pixel 00 in the
 //4x4 matrix. DIGIT is the main parent node.
 enum nodes{ P00=0,  P01=1,  P02=2,  P03=3,
             P10=4,  P11=5,  P12=6,  P13=7,
             P20=8,  P21=9,  P22=10, P23=11,
             P30=12, P31=13, P32=14, P33=15,
             DIGIT=16                     };


 //TRAIN DATASET
 //The file pendigits.tra represent the training
 //dataset, it is composed of 7494 samples
 std::vector<std::vector<unsigned int>> dataset_vector;
 //The dataset is supposed to be in the same folde
 //of the executable.
 std::string train_path = "./pendigits.tra";
 dataset_vector = CreateDatasetFromFile(train_path);

 //TEST DATASET
 //The test dataset is used in the test phase to
 //check the accuracy of the classifier.
 //It is composed of 3498 samples.
 std::vector<std::vector<unsigned int>> test_vector;
 std::string test_path = "./pendigits.tes";
 test_vector = CreateDatasetFromFile(test_path);

 //Creation of the network.
 //A vector of integers is created.
 //Each integer represent the maximum
 //number of states of each node.
 std::cout << "Network Creation" << std::endl;
 std::vector<unsigned int> nodes_vector;
 for(unsigned int i=0; i<16; i++){
  nodes_vector.push_back(100);
 }
  nodes_vector.push_back(10);

 bayonet::Bayesnet myNet(nodes_vector);

 std::cout << "Adding Edges" << std::endl;
 for(unsigned int i=0; i<16; i++){
  myNet.AddEdge(16 , i);
 }


 //The Maximum Likelihood Estimator is defined.
 //The MLE is a way to teach the network.
 //It permits to set the values of the CPTs
 //using a dataset as input.
 std::cout << "The ML estimator is learning the network" << std::endl;
 bayonet::MaximumLikelihoodLearning myEstimator;
 //bayonet::Bayesnet myUpdatedNet = myEstimator.ReturnUpdatedNetwork(myNet, dataset_vector);
 myNet = myEstimator.ReturnUpdatedNetwork(myNet, dataset_vector);


 unsigned int digit_value = 0;
 unsigned int returned_value = 0;
 double success_counter = 0;
 //(starting from zero) 3497 is the maximum number
 //of samples we are dealing with.
 unsigned int test_trial = 3497; 


 //Setting the evidence and calculating the probabilities.
 //This is a cycle through the test dataset.
 for(unsigned int test_counter=0; test_counter<test_trial; test_counter++){
  auto sample_vector = test_vector[test_counter];
  digit_value = sample_vector[16];

  //For each children node the evidence is
  //set using the data parsed from the test
  //dataset.
  myNet[P00].SetEvidence(sample_vector[0]);
  myNet[P01].SetEvidence(sample_vector[1]);
  myNet[P02].SetEvidence(sample_vector[2]);
  myNet[P03].SetEvidence(sample_vector[3]);
  myNet[P10].SetEvidence(sample_vector[4]);
  myNet[P11].SetEvidence(sample_vector[5]);
  myNet[P12].SetEvidence(sample_vector[6]);
  myNet[P13].SetEvidence(sample_vector[7]);
  myNet[P20].SetEvidence(sample_vector[8]);
  myNet[P21].SetEvidence(sample_vector[9]);
  myNet[P22].SetEvidence(sample_vector[10]);
  myNet[P23].SetEvidence(sample_vector[11]);
  myNet[P30].SetEvidence(sample_vector[12]);
  myNet[P31].SetEvidence(sample_vector[13]);
  myNet[P32].SetEvidence(sample_vector[14]);
  myNet[P33].SetEvidence(sample_vector[15]);

  //The Belief Propagator is defined.
  //It permits to make the exact inference,
  //is very fast and can be used here
  //because the naive Bayesian network
  //is a single-connected network.
  bayonet::BeliefPropagator myPropagator;
  myPropagator.InitialTree(myNet);
  myPropagator.UpdateTree(myNet);
  bayonet::MarginalProbabilityTable myMPT = myPropagator.ReturnMarginalProbabilityTable();

  //After the inference process it
  //returns the state with the associate
  //higest probability for the main node.
  returned_value = myMPT.ReturnMostProbableState(DIGIT);

  //Printing the results of the inference.
  //If the returned value is different from
  //the expected value an asterisk * is placed
  //near the number of the sample analized.
  if(returned_value == digit_value) std::cout << "=== " << test_counter << " ====" << std::endl;
  else std::cout << "=== *" << test_counter << " ===" << std::endl;
  std::cout << "DIGIT IS: " << digit_value;
  std::cout << std::endl;
  std::cout << "RETURNED: " << returned_value << std::endl;
  std::cout << std::endl;

  if(digit_value == returned_value) success_counter++;
 }

 //The success rate is shown here.
 //Once parsed the whole test dataset the final
 //accuracy obtained is 77.4%
 double success_rate = (success_counter * 100.0) / (double)test_trial;
 std::cout << "SUCCESS RATE: " << success_rate << "%" << std::endl;
 std::cout << std::endl;

 return 0;
}
