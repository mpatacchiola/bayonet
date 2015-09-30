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

#ifndef SAMPLER_H
#define SAMPLER_H

#include <initializer_list>
#include <map>
#include<vector>
#include <memory>
#include"Bayesnet.h"

namespace bayonet{

/** \class Sampler
*   \brief A generic class that cannot be used for real. It is a framework for derived classes.
*
*  
**/
class Sampler {

public:
virtual std::vector<unsigned int> ReturnSample(Bayesnet) = 0;
virtual std::vector<std::vector<unsigned int>> AccumulateSamples(Bayesnet, unsigned int cycles) = 0;

protected:
std::vector<unsigned int> mSampleVector;

};

/** \class RejectionSampler
*   \brief
*
*  
**/
class RejectionSampler: public Sampler {

public:
RejectionSampler();
~RejectionSampler();
std::vector<unsigned int> ReturnSample(Bayesnet);
std::vector<std::vector<unsigned int>> AccumulateSamples(Bayesnet, unsigned int cycles);

private:

};

/** \class GibbsSampler
*   \brief
*
*  
**/
class GibbsSampler: public Sampler{

public:
GibbsSampler();
~GibbsSampler();
std::vector<unsigned int> ReturnSample(Bayesnet);
std::vector<std::vector<unsigned int>> AccumulateSamples(Bayesnet, unsigned int cycles);

private:

};




}


#endif // SAMPLER_H













