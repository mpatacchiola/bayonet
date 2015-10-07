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

#ifndef REJECTIONSAMPLER_H
#define REJECTIONSAMPLER_H

#include <initializer_list>
#include <map>
#include<vector>
#include <memory>
#include"Bayesnet.h"
#include"JointProbabilityTable.h"

namespace bayonet{


/** \class RejectionSampler
*   \brief
*
*  
**/
class RejectionSampler {

public:
RejectionSampler();
~RejectionSampler();
std::vector<unsigned int> ReturnSample(bayonet::Bayesnet& net);
std::vector<std::vector<unsigned int>> AccumulateSamples(Bayesnet&, unsigned int cycles);
std::vector<std::vector<unsigned int>> AccumulateAndDiscardSamples(Bayesnet&, unsigned int cycles);
void PrintSample(bayonet::Bayesnet& net, unsigned int cycles = 1);
JointProbabilityTable ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles);


private:

};



}

#endif // REJECTIONSAMPLER_H













