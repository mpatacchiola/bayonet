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


private:

struct parameters{
 unsigned int index;
 unsigned int states;
 std::vector<double> belief_vector;
 std::vector<double> pi_messages_vector;
 std::vector<double> lambda_messages_vector;
};



};



}

#endif // BELIEFPROPAGATOR_H













