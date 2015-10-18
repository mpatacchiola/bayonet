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
* for making inferences on a Bayesian network. 
*
**/
class BeliefPropagator {

public:
 BeliefPropagator();
 ~BeliefPropagator();

 JointProbabilityTable ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles);




 struct parameters{
  double belief;
  double pi_value;
  double lambda_value;
  double pi_message;
  double lambda_message;
 };


 void InitialTree(Bayesnet& net);
 void UpdateTree(Bayesnet& net);
 double ReturnPiMessage(bayonet::Bayesnet& net, unsigned int Y, unsigned int X, unsigned int X_state);
 double ReturnLambdaMessage(bayonet::Bayesnet& net, unsigned int Y, unsigned int X, unsigned int X_state);
 void Print();

private:
 std::vector<std::vector<parameters>> parametersVector;

};

}

#endif // BELIEFPROPAGATOR_H













