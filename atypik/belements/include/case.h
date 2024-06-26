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
#ifndef CASE_H
#define CASE_H

#include "expression.h"
#include "instruction.h"

namespace belem {
class Case : public Instruction {
public:
    /*!
     * \brief Construct a B case from a value, some choices, consequences
     * and an alternative
     * \param value
     * The value
     * \param choices
     * The choices
     * \param consequences
     * The consequences
     * \param alternative
     * The alternative
     */
    Case(Expression::shared_ptr value,
         std::vector<Expression::shared_ptr> choices,
         std::vector<Instruction::shared_ptr> consequences,
         Instruction::shared_ptr alternative);
    std::string format() override;

private:
    /*!
     * \brief The value
     */
    Expression::shared_ptr value_;
    /*!
     * \brief The choices
     */
    std::vector<Expression::shared_ptr> choices_;
    /*!
     * \brief The consequences
     */
    std::vector<Instruction::shared_ptr> consequences_;
    /*!
     * \brief The alternative
     */
    Instruction::shared_ptr alternative_;
};
}

#endif // CASE_H
