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
*  This class is a container for joint probabilities. 
*  It can be used as a generic container or it can be associated with a Bayesian network.
**/
class JointProbabilityTable {

 public:
  JointProbabilityTable();
  JointProbabilityTable(std::vector<unsigned int> variablesTotStatesVector);
  ~JointProbabilityTable();

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
