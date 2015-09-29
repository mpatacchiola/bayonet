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

#ifndef BAYESNET_H
#define BAYESNET_H

#include <memory> //shared_ptr
#include"Bayesnode.h"
#include"JointProbabilityTable.h"

namespace bayonet{

/** \class Bayesnet
*   \brief This class represents the whole Bayesian network.
*  
*   ---
**/
class Bayesnet{

 public:

  JointProbabilityTable jointTable;

  //Bayesnode();
  Bayesnet(std::vector<unsigned int> nodesTotStatesVector);
  ~Bayesnet();

  std::shared_ptr<Bayesnode> operator[](unsigned int index);

  bool AddEdge(unsigned int FirstNode, unsigned int SecondNode); //TODO
  bool RemoveEdge(unsigned int FirstNode, unsigned int SecondNode);
  bool HasEdge(unsigned int FirstNode, unsigned int SecondNode);

  unsigned int ReturnNumberOfNodes();
  unsigned int ReturnNumberOfEdges(); //TODO
  unsigned int ReturnNumberOfStates(); //TODO
  double ReturnAverageMarkovBlanketSize();

  std::list<unsigned int> ReturnOutEdges(unsigned int index);
  std::list<unsigned int> ReturnInEdges(unsigned int index);
  unsigned int ReturnNumberOutEdges(unsigned int index);
  unsigned int ReturnNumberInEdges(unsigned int index);

  bool IsTree; //TODO
  bool IsPolytree(); //TODO
  bool IsMultiConnected(); //TODO
  int ReturnNetworkType(); //TODO

  bool IsRoot();  //TODO
  bool IsLeaf(); //TODO
  bool ReturnMarkovBlanketSize(); //TODO

  const std::vector<std::shared_ptr<Bayesnode>>& ReturnNodesVector();
  void FillJointProbabilityTable();

 private:
  std::vector<std::shared_ptr<Bayesnode>> nodesVector;
  std::shared_ptr<JointProbabilityTable> spJointTable;
  
};

}

#endif // BAYESNET_H
