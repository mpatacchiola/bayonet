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

#ifndef MARGINALPROBABILITYTABLE_H
#define MARGINALPROBABILITYTABLE_H

#include<vector>
#include <memory>

namespace bayonet{

/** \class MarginalProbabilityTable
 *  \brief A table containing the marginal probabilities of random variables.
 *
 * The marginal table express the probabilities of each state for each
 * random variable. The discrete distribution is represented as a vector
 * where each element is a probability associated with a state.
 * 
**/
class MarginalProbabilityTable {

 public:
  MarginalProbabilityTable(unsigned int totVariables, unsigned int totStates);
  MarginalProbabilityTable(std::vector<unsigned int> variablesTotStatesVector);
  ~MarginalProbabilityTable();

 bool SetProbability(unsigned int variableIndex, unsigned int stateIndex, double probability);
 bool AddToProbability(unsigned int variableIndex, unsigned int stateIndex, double probability);
 double GetProbability(unsigned int variableIndex, unsigned int stateIndex);

 unsigned int ReturnMostProbableState(unsigned int variableIndex);

 bool SetProbabilities(unsigned int index, std::vector<double> probabilitiesVector);
 std::vector<double> GetProbabilities(unsigned int index);

 void ResetProbabilities();
 void NormalizeProbabilities();

 void Print();
 void PrintVariable(unsigned int index);

 private:
  std::vector<std::vector<double>> marginalTable;

};

}

#endif // MARGINALPROBABILITYTABLE_H
