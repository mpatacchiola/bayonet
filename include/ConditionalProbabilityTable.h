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

#ifndef CONDITIONALPROBABILITYTABLE_H
#define CONDITIONALPROBABILITYTABLE_H

#include <initializer_list>
#include <map>
#include <vector>
#include <memory>

namespace bayonet{

/** \class ConditionalProbabilityTable
 *  \brief A table containing the conditional probabilities of random variables.
 *
 * To quantify the relationships between the nodes it is necessary to specify
 * a conditional distribution for each node given its parents. Using discrete
 * random variables the best way to represent such a relationship is the 
 * conditional probability table (CPT). The CPT of a node contains all the
 * possible combinations of values (instantiation) of the parents nodes. For
 * each instantiation it is necessary to specify the probabilities that the 
 * child node assumes each of its values. 
 * Bayonet store the CPT as a std::map, where the key is a vector of integrers
 * representing the states of the parents node, and the value is a vector of 
 * double representing the probabilities of the variable states.
 * 
**/
class ConditionalProbabilityTable {

 public:
  ConditionalProbabilityTable(unsigned int NodeStatesNumber);
  ConditionalProbabilityTable(unsigned int NodeStatesNumber, std::vector<unsigned int> parentsStatesList);
  ~ConditionalProbabilityTable();

  std::pair<std::vector<unsigned int>, std::vector<double>> ReturnRow(unsigned int index);
  std::vector<unsigned int> ReturnParentsState(unsigned int index);

  std::vector<unsigned int> FindParentState(unsigned int parentIndex, unsigned int parentState);

  double GetProbability(unsigned int variableState, std::vector<unsigned int> parentsStates);
  std::vector<double> GetProbabilities(std::vector<unsigned int> parentsStates);
  std::vector<double> GetProbabilities(unsigned int variableState, unsigned int parentIndex, unsigned int parentsState);
  bool SetProbabilities(std::vector<unsigned int> parentsStates, std::vector<double> probabilities);
  void Print();
  void PrintProbabilities(std::vector<unsigned int> parentsStates);

  void NormalizeProbabilities();
  void RandomizeProbabilities();

  unsigned int ReturnSample(std::vector<unsigned int> parentsStates);

  void AddVariable(unsigned int totStates);
  void Clear();

  unsigned int ReturnRowsNumber();
  unsigned int ReturnColumnsNumber();


 private:
  std::vector<unsigned int> mTotalParentsStates;
  std::map<std::vector<unsigned int>,std::vector<double> > conditionalMap;

  void FillMap(unsigned int NodeStatesNumber, std::vector<unsigned int> parentsStates);

};

}

#endif // CONDITIONALPROBABILITYTABLE_H
