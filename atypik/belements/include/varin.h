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
#ifndef VARIN_H
#define VARIN_H

#include "expression.h"
#include "instruction.h"

namespace belem {
class VarIn : public Instruction {
public:
    /*!
     * \brief Construct a B var in instruction
     * \param variables
     * The variables introduced by the instruction
     * \param body
     * The body of the instruction
     */
    VarIn(std::vector<Expression::shared_ptr> variables,
          Instruction::shared_ptr body);
    std::string format() override;

private:
    /*!
     * \brief The variables introduced by the instruction
     */
    std::vector<Expression::shared_ptr> variables_;
    /*!
     * \brief The body of the instruction
     */
    Instruction::shared_ptr body_;
};
}

#endif // VARIN_H
