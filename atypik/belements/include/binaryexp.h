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
#ifndef BINARYEXP_H
#define BINARYEXP_H

#include "expression.h"

namespace belem
{
class BinaryExp : public Expression
{
public:
    /*!
     * \brief Construct a binary expression from a two operands and an
     * operator
     * \param left
     * The left operand
     * \param op
     * The operator
     * \param right
     * The right operand
     * \param position
     * The position of the expression in the bxml file
     */
    BinaryExp(Expression::shared_ptr left,
              std::string op,
              Expression::shared_ptr right,
              bxml::Position::shared_ptr position);
    std::string format() override;

private:
    /*!
     * \brief The left operand
     */
    Expression::shared_ptr left_;
    /*!
     * \brief The right operand
     */
    Expression::shared_ptr right_;
    /*!
     * \brief The operator
     */
    std::string op_;
};
}

#endif // BINARYEXP_H
