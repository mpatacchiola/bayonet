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

#include <memory> //shared_ptr
#include"Bayesnode.h"

namespace bayonet{

class Bayesnet{

 public:
  //Bayesnode();
  Bayesnet(unsigned int numberOfNodes, unsigned int numberOfStates);

  ~Bayesnet();

  bool AddConnection(unsigned int FirstNode, unsigned int SecondNode);
  bool RemoveConnection(unsigned int FirstNode, unsigned int SecondNode);

  std::vector<unsigned int> ReturnSampleVector();


 private:
  std::vector<std::shared_ptr<Bayesnode>> nodesVector;

};

}
