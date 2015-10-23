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

#ifndef MAXIMUMLIKELIHOODLEARNING_H
#define MAXIMUMLIKELIHOODLEARNINGR_H

#include<vector>
#include <memory>
#include"Bayesnet.h"


namespace bayonet{


/** \class MaximumLikelihoodLearning
*   \brief Implementation of Maximum Likelihood Estimator (MLE).
*
*  
* The MaximumLikelihoodLearning permits to estimate the parameters of a
* Bayesian network given a set of training data. The parameters of a BN are the 
* Conditional Table entries associated with each node. The MLE for discrete BN 
* simply count how many times each assignments of the variables appears in
* the training dataset.
*
* "Probabilistic Graphical Models. Principles and Techniques." (Daphne Koller and Nir Friedman.) chapter 17.
*
**/
class MaximumLikelihoodLearning {

public:
 MaximumLikelihoodLearning();
 ~MaximumLikelihoodLearning();

 Bayesnet ReturnUpdatedNetwork(Bayesnet net, std::vector<std::vector<unsigned int>>& trainingDataset);


private:


};

}

#endif // MAXIMUMLIKELIHOODLEARNING_H













