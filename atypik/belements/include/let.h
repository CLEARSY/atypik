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
#ifndef LET_H
#define LET_H

#include "expression.h"
#include "instruction.h"
#include "predicate.h"

namespace belem {
class Let : public Instruction {
public:
    /*!
     * \brief Construct a B let instruction
     * \param variables
     * The variables introduced by the let
     * \param values
     * The values of the introduced variables
     * \param body
     * The body of the instruction
     */
    Let(std::vector<Expression::shared_ptr> variables,
        std::vector<Expression::shared_ptr> values,
        Instruction::shared_ptr body);
    std::string format() override;

private:
    /*!
     * \brief The variables introduced by the let instruction
     */
    std::vector<Expression::shared_ptr> variables_;
    /*!
     * \brief The values of the introduced variables
     */
    std::vector<Expression::shared_ptr> values_;
    /*!
     * \brief The body of the instruction
     */
    Instruction::shared_ptr body_;
};
}

#endif // LET_H
