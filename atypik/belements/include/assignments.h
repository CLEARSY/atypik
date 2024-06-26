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
#ifndef ASSIGNMENTS_H
#define ASSIGNMENTS_H

#include "expression.h"
#include "instruction.h"

#include <vector>

namespace belem
{
class Assignments : public Instruction
{
public:
    /*!
     * \brief Construct B assignments from a vector of variables
     * and a vector of values
     * \param variables
     * The vector of variables
     * \param values
     * The vector of values
     */
    Assignments(std::vector<Expression::shared_ptr> variables,
                std::vector<Expression::shared_ptr> values);
    std::string format() override;

private:
    /*!
     * \brief The variables
     */
    std::vector<Expression::shared_ptr> variables_;
    /*!
     * \brief The values
     */
    std::vector<Expression::shared_ptr> values_;
};
}

#endif // ASSIGNMENTS_H
