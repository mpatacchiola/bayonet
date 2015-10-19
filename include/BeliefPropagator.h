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

#ifndef BELIEFPROPAGATOR_H
#define BELIEFPROPAGATORR_H

#include<vector>
#include <memory>
#include"Bayesnet.h"
#include"JointProbabilityTable.h"

namespace bayonet{


/** \class BeliefPropagator
*   \brief Implementation of the belief propagation algorithm.
*
*  
* The BeliefPropagator class permits to use the Kim-Pearl message passing algorithm
* for making exact inferences in a Bayesian network. The Kim-Pearl algorithm uses a set of
* parameters, called pi-values and lambda-values, and a set of messages called
* pi-message and lambda-message, for answering inference queries. 
* This algorithm can be used only if the network is single connected.
* The algorithm is quite complicated, a good description can be found in:
*
* "Bayesian Artificial Intelligence" (Kevin B. Korb and Ann E. Nicholson) chapter 3.
* "Learning Bayesian Networks" (Richard E. Neapolitan) chapter 3.
*
**/
class BeliefPropagator {

public:
 BeliefPropagator();
 ~BeliefPropagator();

 void InitialTree(Bayesnet& net);
 void UpdateTree(Bayesnet& net);
 void Print();

 JointProbabilityTable ReturnJointProbabilityTable(bayonet::Bayesnet& net);

private:

 struct parameters{
  double belief;
  double pi_value;
  double lambda_value;
 };

 std::vector<std::vector<parameters>> parametersVector;
 double ReturnPiMessage(bayonet::Bayesnet& net, unsigned int Y, unsigned int X, unsigned int X_state);
 double ReturnLambdaMessage(bayonet::Bayesnet& net, unsigned int Y, unsigned int X, unsigned int X_state);
 void SetPiValues(bayonet::Bayesnet& net, unsigned int X);
 void SetLambdaValues(bayonet::Bayesnet& net, unsigned int X);
 void SetBelief(bayonet::Bayesnet& net, unsigned int X);
};

}

#endif // BELIEFPROPAGATOR_H













