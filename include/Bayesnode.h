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

#ifndef BAYESNODE_H
#define BAYESNODE_H

#include <vector>
#include<list>
#include <string>
#include <memory>
#include "ConditionalProbabilityTable.h"

namespace bayonet{

/** \class Bayesnode
*   \brief This class represents the single node of a Bayesian network.
*  
*   ---
**/
class Bayesnode {

 public:

  ConditionalProbabilityTable conditionalTable;

  //Bayesnode();
  Bayesnode(unsigned int numberOfStates);

  ~Bayesnode();

  unsigned int ReturnNumberOfStates();

  void SetLabel(std::string);
  std::string GetLabel();
  void SetNumericLabel(int);
  int GetNumericLabel();

  bool AddIncomingEdge(unsigned int,  unsigned int);
  bool RemoveIncomingEdge(unsigned int);
  const std::list<unsigned int>&  ReturnAdjacencyList();
  bool HasIncomingEdgeFrom(unsigned int);
  unsigned int ReturnNumberIncomingEdges();

  bool SetAsEvidence(unsigned int evidenceState);
  int IsEvidence();

  //unsigned int ReturnMarkovBlanketSize();
  std::shared_ptr<ConditionalProbabilityTable> ReturnPointerToConditionalTable();

 private:
  int mEvidence;
  unsigned int mNumberOfStates;
  std::string mNodeLabel;
  int mNodeNumericLabel;
  std::list<unsigned int> adjacencyList;


};

}

#endif // BAYESNODE_H
