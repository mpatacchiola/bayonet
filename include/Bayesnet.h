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
#include <functional>

namespace bayonet{

/** \class Bayesnet
*   \brief This class represents the whole Bayesian network.
*  
*   ---
**/
class Bayesnet{

 public:

  Bayesnet(std::vector<unsigned int> nodesTotStatesVector);
  ~Bayesnet();

  Bayesnode& operator[](unsigned int index);

  bool AddEdge(unsigned int FirstNode, unsigned int SecondNode);
  bool RemoveEdge(unsigned int FirstNode, unsigned int SecondNode);
  bool HasEdge(unsigned int FirstNode, unsigned int SecondNode);

  unsigned int ReturnNumberOfNodes();
  unsigned int ReturnNumberOfEdges();

  std::list<unsigned int> ReturnOutEdges(unsigned int index);
  std::list<unsigned int> ReturnInEdges(unsigned int index);
  unsigned int ReturnNumberOutEdges(unsigned int index);
  unsigned int ReturnNumberInEdges(unsigned int index);
  std::list<unsigned int> ReturnTopologicalList();
  std::vector<unsigned int> ReturnTotalStates();
  std::vector<unsigned int> ReturnNotEvidenceNodes();
  std::vector<unsigned int> ReturnEvidenceNodes();

  double GetNodeProbability(unsigned int index, std::vector<unsigned int> variablesStatesVector);

  void ResetAllColours();

  //bool IsTree; //TODO
  //bool IsPolytree(); //TODO
  //bool IsMultiConnected(); //TODO
  //int ReturnNetworkType(); //TODO

  bool IsRoot(unsigned int);
  bool IsLeaf(unsigned int); //TODO
  //unsigned int ReturnMarkovBlanketSize(unsigned int index); //TODO
  //double ReturnAverageMarkovBlanketSize(unsigned int index); //TODO

  const std::vector<Bayesnode>& ReturnNodesVector();
  void FillJointProbabilityTable();

  std::list<unsigned int> BreadthFirstSearch(unsigned int startingNode);
  void DepthFirstSearch(unsigned int startingNode, std::shared_ptr<std::list<unsigned int>> spToList, bool resetColours = true);

 private:
  //std::vector<std::shared_ptr<Bayesnode>> nodesVector;
  std::vector<Bayesnode> nodesVector;
  
};

}

#endif // BAYESNET_H
