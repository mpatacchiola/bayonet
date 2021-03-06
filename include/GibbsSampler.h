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

#ifndef GIBBSSAMPLER_H
#define GIBBSSAMPLER_H

#include<vector>
#include"Bayesnet.h"
#include"JointProbabilityTable.h"
#include"MarginalProbabilityTable.h"


namespace bayonet{


/** \class GibbsSampler
*   \brief Implementation of the Gibbs sampler. 
* 
* The Gibbs sampler is part of the Markov Chain Monte Carlo (MCMC) methods.
* These family of methods work differently from rejection and LW sampling.
* Each sample is generated by making a random change to the preceding one.
* The algorithm starts with a random state where only the evidence variables
* are fixed at their observed values. After the initialization, the sampling
* is done conditioned on the current values of the variables in the Markov
* blanket of the node. The algorithm then continue moving at random,
* flipping one variable at a time but leaving the evidences unchanged.
*
**/
class GibbsSampler {

public:
GibbsSampler();
~GibbsSampler();

std::vector<unsigned int> ReturnSample(bayonet::Bayesnet& net);
std::vector<unsigned int> ReturnSample(bayonet::Bayesnet& net, std::vector<unsigned int> startingVector);

std::vector<std::vector<unsigned int>> AccumulateSamples(Bayesnet& net, unsigned int cycles);

void PrintSample(bayonet::Bayesnet& net, unsigned int cycles = 1);
JointProbabilityTable ReturnJointProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles);
MarginalProbabilityTable ReturnMarginalProbabilityTable(bayonet::Bayesnet& net, unsigned int cycles);

private:
int ReturnInteger(const int minRange, const int maxRange);

};



}

#endif // GIBBSSAMPLER_H













