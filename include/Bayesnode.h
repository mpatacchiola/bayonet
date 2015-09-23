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

#include <vector>
#include <string>
#include <memory>

namespace bayonet{

class Bayesnode {

 public:

  //Bayesnode();
  Bayesnode(unsigned int numberOfStates);

  ~Bayesnode();

  void PrintStates();
  unsigned int ReturnStatesNumber();
  double ReturnStatesSum();
  bool SetStateValue(unsigned int index, double value);
  double GetStateValue(unsigned int index);
  void NormalizeValues();
  bool AddIncomingConnection(std::shared_ptr<Bayesnode>);
  bool AddOutgoingConnection(std::shared_ptr<Bayesnode>);
  unsigned int ReturnSample();
  bool IsRoot();
  bool IsLeaf();

 private:
  std::string nodeLabel;
  std::vector<double> valuesVector;
  std::vector<std::weak_ptr<Bayesnode>> incomingVector;
  std::vector<std::weak_ptr<Bayesnode>> outgoingVector; 

};

}
