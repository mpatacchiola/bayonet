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

#ifndef JOINTPROBABILITYTABLE_H
#define JOINTPROBABILITYTABLE_H

#include <initializer_list>
#include <map>
#include<vector>
#include <memory>

namespace bayonet{

/** \class JointProbabilityTable
 *   \brief A table containing the joint probabilities of random variables.
 *
 *  Why a Joint Conditional Table class is important? 
 *
 *  "Most commonly, BNs are considered to be representations of joint probability distri-
 *  butions. There is a fundamental assumption that there is a useful underlying structure
 *  to the problem being modeled that can be captured with a BN, i.e., that not every
 *  node is connected to every other node. If such domain structure exists, a BN gives
 *  a more compact representation than simply describing the probability of every joint
 *  instantiation of all variables. Sparse Bayesian networks (those with relatively few
 *  arcs, which means few parents for each node) represent probability distributions in a
 *  computationally tractable way." 
 *  (Bayesian Artificial Intelligence, Second Edition, 2011)
**/
class JointProbabilityTable {

 public:
  JointProbabilityTable();
  JointProbabilityTable(std::vector<unsigned int> variablesTotStatesVector);
  ~JointProbabilityTable();

  std::pair<std::vector<unsigned int>, double> ReturnRow(unsigned int index);

  double ReturnMarginal(unsigned int variableIndex, unsigned int variableState);
  std::vector<unsigned int> ReturnKey(unsigned int index);
  double GetProbability(std::vector<unsigned int> variablesStatesVector);
  bool SetProbability(std::vector<unsigned int> variablesStatesVector, double probability);
  bool AddToProbability(std::vector<unsigned int> variablesStatesVector, double valueToAdd);
  void Print();
  void PrintProbability(std::vector<unsigned int> variablesStatesVector);
  void PrintMarginals();
  void PrintMarginal(unsigned int variableIndex);

  void NormalizeProbabilities(double alpha=0);
  void RandomizeProbabilities();
  void ResetProbabilities();

  void AddVariable(unsigned int totStates);
  void Clear();

  unsigned int ReturnRowsNumber();
  const std::map<std::vector<unsigned int>,double>& ReturnJointMap();


 private:
  std::vector<unsigned int> mVariablesTotStatesVector;
  std::map<std::vector<unsigned int>,double> mJointMap;

  void FillMap(std::vector<unsigned int> variablesTotStatesVector);

};

}

#endif // JOINTPROBABILITYTABLE_H
