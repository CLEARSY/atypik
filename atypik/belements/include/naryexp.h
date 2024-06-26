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
#ifndef NARYEXP_H
#define NARYEXP_H

#include "expression.h"

#include <vector>

namespace belem
{
class NaryExp : public Expression
{
public:
    /*!
     * \brief Construct an expression of arity n
     * \param expressions
     * The operands of the n-arity expression
     * \param op
     * The operator
     */
    NaryExp(std::vector<Expression::shared_ptr> operands, std::string op,
            bxml::Position::shared_ptr position);
    std::string format() override;

private:
    /*!
     * \brief The operands of the expression
     */
    std::vector<Expression::shared_ptr> operands_;
    /*!
     * \brief The operator
     */
    std::string op_;
};
}

#endif // NARYEXP_H
