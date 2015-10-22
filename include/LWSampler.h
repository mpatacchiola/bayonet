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

#ifndef LWSAMPLER_H
#define LWSAMPLER_H

#include<vector>
#include"Bayesnet.h"
#include"JointProbabilityTable.h"
#include"MarginalProbabilityTable.h"

namespace bayonet{


/** \class LWSampler
*   \brief Implementation of the Likelihood-Weighted sampler. 
* 
* Using a rejection sampler with high dimensional random variables
* is problematic because many samples get lost. In this case a
* Likelihood-Weighted sampler is more efficient. The samples are "forced"
* to assume the appropriate value according to the inference.
*
**/
class LWSampler {

public:
LWSampler();
~LWSampler();

std::pair< std::vector<unsigned int>, double> ReturnSample(bayonet::Bayesnet& net);

std::vector<std::pair<std::vector<unsigned int>, double>> AccumulateSamples(Bayesnet&, unsigned int cycles);

void PrintSample(bayonet::Bayesnet& net, unsigned int cycles = 1);
JointProbabilityTable ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles);
MarginalProbabilityTable ReturnMarginalProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles);


private:

};



}

#endif // LWSAMPLER_H













