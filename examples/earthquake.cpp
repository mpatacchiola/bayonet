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

/* 
 * This example is taken from Pearl (1988). The network has 5 boolean nodes and 
 * is a single-connected network that create an X (see figure below). The original
 * example statement is the following:
 * 
 * You have a new burglar alarm installed. It reliably detects
 * burglary, but also responds to minor earthquakes. Two neighbors, John and Mary,
 * promise to call the police when they hear the alarm. John always calls when he
 * hears the alarm, but sometimes confuses the alarm with the phone ringing and calls
 * then also. On the other hand, Mary likes loud music and sometimes doesn’t hear the
 * alarm. Given evidence about who has and hasn’t called, you’d like to estimate the
 * probability of a burglary (from Pearl (1988)).
 * 
 * 
 *   [BURGLARY]   [EARTHQUAKE]
 *         \\       //
 *          \\     // 
 *           \/   \/
 *           [ALARM]
 *          //     \\
 *         //       \\
 *         \/       \/
 *      [JOHN]    [MARY]
 *
 * 
*/


#include <iostream>
#include<bayonet/Bayesnet.h>
#include<bayonet/Bayesnode.h>
#include<bayonet/ConditionalProbabilityTable.h>
#include<bayonet/JointProbabilityTable.h>
#include<bayonet/LWSampler.h>
#include<bayonet/GibbsSampler.h>

int main()
{

 //This enum is a way to deal with the node in a more confortable way.
 //Instead of using numbers we are going to use words.
 enum node{
  BURGLARY=0,
  EARTHQUAKE=1,
  ALARM=2,
  JOHN=3,
  MARY=4
 };

  //It creates a net with 5 nodes, each node can assume 2 states
 bayonet::Bayesnet myNet({2, 2, 2, 2, 2});

 //It defines the number of iteration to use for the sampler
 //and the sampler we are going to use.
 unsigned int iterations = 50000;
 bayonet::LWSampler myLWSampler;
 //bayonet::GibbsSampler myGibbsSampler; //A Gibbs sampler can be used too


 //Setting the Bayesian network.
 //There are 5 boolean nodes: BURGLARY, EARTHQUAKE, ALARM, JOHN, MARY
 myNet.AddEdge(BURGLARY, ALARM);
 myNet.AddEdge(EARTHQUAKE, ALARM);
 myNet.AddEdge(ALARM, JOHN);
 myNet.AddEdge(ALARM, MARY);

 //Setting the Conditional tables for each node.
 //The conditional table contains the probabilities associated with each
 //state of each parent of the current node.
 myNet[BURGLARY]->conditionalTable.SetProbabilities({}, {0.99, 0.01});

 myNet[EARTHQUAKE]->conditionalTable.SetProbabilities({}, {0.98, 0.02});

 //Attention: true and false are used to make the code clear,
 //the function takes integers, here a cast bool>int is automatically done.
 myNet[ALARM]->conditionalTable.SetProbabilities({true,true}, {0.05, 0.95});
 myNet[ALARM]->conditionalTable.SetProbabilities({true,false}, {0.06, 0.94});
 myNet[ALARM]->conditionalTable.SetProbabilities({false,true}, {0.71, 0.29});
 myNet[ALARM]->conditionalTable.SetProbabilities({false,false}, {0.999, 0.001});

 myNet[JOHN]->conditionalTable.SetProbabilities({true}, {0.10, 0.90});
 myNet[JOHN]->conditionalTable.SetProbabilities({false}, {0.95, 0.05});

 myNet[MARY]->conditionalTable.SetProbabilities({true}, {0.30, 0.70});
 myNet[MARY]->conditionalTable.SetProbabilities({false}, {0.99, 0.01});



 //The output of the sampler is a JointTable, that is a useful
 //way to deal with joint probabilities. 
 //Here the inference is obtained through sampling.
 bayonet::JointProbabilityTable myJointTable = myLWSampler.ReturnJointProbabilityTable(myNet,iterations);

 //Here we are going to print the result of the sampling
 std::cout << std::endl << "======= Initial conditions without evidences  =======" << std::endl;
 std::cout << std::endl << "------------- BURGLARY -------------" << std::endl;
 myJointTable.PrintMarginal(BURGLARY );
 std::cout << std::endl << "------------ EARTHQUAKE ------------" << std::endl;
 myJointTable.PrintMarginal(EARTHQUAKE);
 std::cout << std::endl << "-------------- ALARM ---------------" << std::endl;
 myJointTable.PrintMarginal(ALARM);
 std::cout << std::endl << "--------------- JOHN --------------" << std::endl;
 myJointTable.PrintMarginal(JOHN);
 std::cout << std::endl << "--------------- MARY --------------" << std::endl;
 myJointTable.PrintMarginal(MARY);
 std::cout << std::endl;

 //In the following sections we are going to play with the nodes,
 //setting up different inference states and looking for the results.
 myNet[JOHN]->SetEvidence(true);
 myJointTable = myLWSampler.ReturnJointProbabilityTable(myNet,iterations);

 std::cout << std::endl << "============ JOHN calls ============" << std::endl;
 std::cout << std::endl << "------------- BURGLARY -------------" << std::endl;
 myJointTable.PrintMarginal(BURGLARY );
 std::cout << std::endl << "------------ EARTHQUAKE ------------" << std::endl;
 myJointTable.PrintMarginal(EARTHQUAKE);
 std::cout << std::endl << "-------------- ALARM ---------------" << std::endl;
 myJointTable.PrintMarginal(ALARM);
 std::cout << std::endl << "--------------- JOHN --------------" << std::endl;
 myJointTable.PrintMarginal(JOHN);
 std::cout << std::endl << "--------------- MARY --------------" << std::endl;
 myJointTable.PrintMarginal(MARY);
 std::cout << std::endl;


 //setting up different inference states and looking for the results.
 myNet[JOHN]->SetEvidence(-1); //disabling the evidence for JOHN node
 myNet[BURGLARY]->SetEvidence(true);

 myJointTable = myLWSampler.ReturnJointProbabilityTable(myNet,iterations);
 std::cout << std::endl << "======= There is a BURGLARY  =======" << std::endl;
 std::cout << std::endl << "------------- BURGLARY -------------" << std::endl;
 myJointTable.PrintMarginal(BURGLARY );
 std::cout << std::endl << "------------ EARTHQUAKE ------------" << std::endl;
 myJointTable.PrintMarginal(EARTHQUAKE);
 std::cout << std::endl << "-------------- ALARM ---------------" << std::endl;
 myJointTable.PrintMarginal(ALARM);
 std::cout << std::endl << "--------------- JOHN --------------" << std::endl;
 myJointTable.PrintMarginal(JOHN);
 std::cout << std::endl << "--------------- MARY --------------" << std::endl;
 myJointTable.PrintMarginal(MARY);
 std::cout << std::endl;


 return 0;
}




