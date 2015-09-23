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

#include"Bayesnet.h"

namespace bayonet{

Bayesnet::Bayesnet(unsigned int numberOfNodes, unsigned int numberOfStates = 2){
 for(unsigned int i=0; i<numberOfNodes; i++){
  auto sp = std::make_shared<Bayesnode>(numberOfStates);
  nodesVector.push_back(sp);
 }
}

Bayesnet::~Bayesnet(){

}


bool Bayesnet::AddConnection(unsigned int firstNode, unsigned int secondNode){
 if(firstNode == secondNode) return false;

 nodesVector[firstNode]->AddOutgoingConnection(nodesVector[secondNode]); 
 nodesVector[secondNode]->AddIncomingConnection(nodesVector[firstNode]);
 
 return true;
}

bool Bayesnet::RemoveConnection(unsigned int firstNode, unsigned int secondNode){
 if(firstNode == secondNode) return false;

 return true;
}

std::vector<unsigned int> Bayesnet::ReturnSampleVector(){

  std::vector<unsigned int> temp_vector;
  temp_vector.reserve(nodesVector.size());

  //iterating each node and collecting samples
 for(auto it = nodesVector.begin(); it != nodesVector.end(); ++it) {
  temp_vector.push_back( (*it)->ReturnSample() );
 }

 return temp_vector;
}



}




