/*
 * A TYPe Inference Kit for B.
 *
 * This file is part of the atypik project.
 * Copyright (c) 2023 CLEARSY
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License version 3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>. 
 *
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the atypik software without
 * disclosing the source code of your own applications.
 *
 */
#ifndef MODELSET_H
#define MODELSET_H

#include <vector>
#include "model.h"

namespace solver {
/*!
 * \brief The ModelSet class is used to contain and solve multiple models
 */
class ModelSet {
public:
    /*!
     * \brief Construct a model set from a vector of models
     * \param models
     * The vector of models
     */
    ModelSet(std::vector<Model::shared_ptr> models);
    /*!
     * \brief Return a solution to the models if they are sat.
     * \return A map of shape variable -> type where variable is a variable of
     * one of the model and type is the infered type. Raises a execption
     * if one of the model is unsat.
     */
    std::unordered_map<Variable::shared_ptr, std::string> solve();
    /*!
     * \brief An accessor on the model of number num
     * \param num
     * The number of the model
     * \return the model of number num
     */
    Model::shared_ptr getModel(int num);

private:
    /*!
     * \brief The models in the set
     */
    std::vector<Model::shared_ptr> models_;
};
}



#endif // MODELSET_H
