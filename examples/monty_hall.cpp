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


//The Monty Hall problem is a probability puzzle based on the American show "Let's make a deal"
//and named after the Canadian-born host Monty Hall (Monte Halparin). One of the formulation of
//the problem is the following:
//
//Suppose you are on a game show, and you are given the choice of three doors: A, B, C. 
//Behind one door is a car, behind the others, goats. You pick a door, say A, and the host (Monty), 
//who knows what's behind the doors, opens another door, say C, which has a goat. He then says to you: 
//"Do you want to pick door B?" Is it to your advantage to switch your choice?
//
//SPOILER: The correct answer in this case is that it is correct to switch, because the probability
//to pick the car is higher.
//This result is very counterintuitive and many people (included PhD professors) find this answer
//absurd. In the following lines of codes we will solve the Monty Hall problem using Bayonet.
//
//The network contains 3 nodes and looks as follows:
//
//   [PLAYER]  [PRIZE]
//       \\     //
//        \\   // 
//         \/ \/
//        [MONTY]
//
//Our goal is to infer which are the probabilities to find the PRIZE behind each door after that PLAYER 
//and MONTY made their choices.

#include<bayonet/Bayesnet.h>
#include<bayonet/Bayesnode.h>
#include<bayonet/ConditionalProbabilityTable.h>
#include<bayonet/JointProbabilityTable.h>
#include<bayonet/RejectionSampler.h>
#include <iostream>

int main()
{
 //There are three nodes, the PLAYER choice, the PRICE position, the MONTY choice
 //Here we create an enum to make the nodes more readable.
 enum node{
  PLAYER=0,
  PRIZE=1,
  MONTY=2
 };

 //The name of the three doors is A, B, C
 enum door{
  A=0,
  B=1,
  C=2
 };

 unsigned int iterations = 80000; //It defines the number of iterations used in the sampler
 bayonet::Bayesnet myNet({3, 3, 3}); //It creates a net with 3 nodes, each node can assume 3 states
 bayonet::RejectionSampler myRejectionSampler; //It defines the rejection sampler

 //There are three nodes in the network, the nodes PLAYER and PRIZE have a connection to the node MONTY
 //Indeed the decision of MONTY is dependent from the choice of the PLAYER and from the position of the PRIZE.
 //Here we add the two edges PLAYER->MONTY and PRIZE->MONTY.
 myNet.AddEdge(PLAYER, MONTY);
 myNet.AddEdge(PRIZE, MONTY);

 //In this section we must define the conditional table for the node MONTY.
 //The conditional table for the two nodes PLAYER and MONTY are initialized
 //automatically when the nodes are created (when the conditional table is 
 //not explicitly defined each state has the same probability).

 //If GUEST choose the door with the prize then MONTY can choose with equal 
 //probability some of the other two doors.
 myNet[MONTY].conditionalTable.SetProbabilities({A, A}, {0.0, 0.5, 0.5}); //PLAYER choose A, PRIZE is in A
 myNet[MONTY].conditionalTable.SetProbabilities({B, B}, {0.5, 0.0, 0.5}); //PLAYER choose B, PRIZE is in B
 myNet[MONTY].conditionalTable.SetProbabilities({C, C}, {0.5, 0.5, 0.0}); //PLAYER choose C, PRIZE is in C

 //if the GUEST choose a door where there is not a prize, then MONTY will 
 //choose the remaining empty door.
 myNet[MONTY].conditionalTable.SetProbabilities({A, B}, {0.0, 0.0, 1.0}); //PLAYER choose A, PRIZE is in B
 myNet[MONTY].conditionalTable.SetProbabilities({A, C}, {0.0, 1.0, 0.0}); //PLAYER choose A, PRIZE is in C
 myNet[MONTY].conditionalTable.SetProbabilities({B, A}, {0.0, 0.0, 1.0}); //PLAYER choose B, PRIZE is in A
 myNet[MONTY].conditionalTable.SetProbabilities({B, C}, {1.0, 0.0, 0.0}); //PLAYER choose B, PRIZE is in C
 myNet[MONTY].conditionalTable.SetProbabilities({C, A}, {0.0, 1.0, 0.0}); //PLAYER choose C, PRIZE is in A
 myNet[MONTY].conditionalTable.SetProbabilities({C, B}, {1.0, 0.0, 0.0}); //PLAYER choose C, PRIZE is in B

 //First we print the marginals for each node and each state. In this
 //moment we do not have any evidence to define, then each outcome
 //is equally like.
 std::cout << std::endl << "======= Initial conditions without evidences  =======" << std::endl;
 bayonet::JointProbabilityTable myJointTable = myRejectionSampler.ReturnJointProbabilityTable(myNet,iterations);
 std::cout << std::endl << "------------- PLAYER -------------" << std::endl;
 myJointTable.PrintMarginal(PLAYER);
 std::cout << std::endl << "------------- PRIZE -------------" << std::endl;
 myJointTable.PrintMarginal(PRIZE);
 std::cout << std::endl << "------------- MONTY -------------" << std::endl;
 myJointTable.PrintMarginal(MONTY);
 std::cout << std::endl;

 //Let's suppose that the PLAYER choose a door, the door A. After
 //that decision we are in a new universe and the probabilities
 //are different from the previous plot.
 std::cout << std::endl << "======= The PLAYER choose the door A =======" << std::endl;
 myNet[PLAYER].SetEvidence(A);
 myJointTable = myRejectionSampler.ReturnJointProbabilityTable(myNet,iterations);
 std::cout << std::endl << "------------- PLAYER -------------" << std::endl;
 myJointTable.PrintMarginal(PLAYER);
 std::cout << std::endl << "------------- PRIZE -------------" << std::endl;
 myJointTable.PrintMarginal(PRIZE);
 std::cout << std::endl << "------------- MONTY -------------" << std::endl;
 myJointTable.PrintMarginal(MONTY);
 std::cout << std::endl;

 //Now let's suppose that MONTY decides to open the door B. This
 //decision generate a third universe with new associated 
 //probabilities. This is the final step and it permits to solve
 //the Monty Hall dilemma.
 std::cout << std::endl << "=======  MONTY opens the door B  =======" << std::endl;
 myNet[MONTY].SetEvidence(B);
 myJointTable = myRejectionSampler.ReturnJointProbabilityTable(myNet,iterations);
 std::cout << std::endl << "------------- PLAYER -------------" << std::endl;
 myJointTable.PrintMarginal(PLAYER);
 std::cout << std::endl << "------------- PRIZE -------------" << std::endl;
 myJointTable.PrintMarginal(PRIZE);
 std::cout << std::endl << "------------- MONTY -------------" << std::endl;
 myJointTable.PrintMarginal(MONTY);
 std::cout << std::endl;

 return 0;
}



