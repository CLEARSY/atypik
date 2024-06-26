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
#ifndef UNARYEXP_H
#define UNARYEXP_H

#include "expression.h"

namespace belem
{
class UnaryExp : public Expression
{
public:
    /*!
   * \brief Construct a unary expression
   * \param op
   * The operator of the expression
   * \param expression
   * The content of the expression
   * \param pos
   * The position in the bxml file
   */
  UnaryExp(std::string op,
           Expression::shared_ptr expression,
           bxml::Position::shared_ptr pos);
  std::string format() override;

private:
  /*!
   * \brief The operator
   */
  std::string op_;
  /*!
   * \brief The content of the expression
   */
  Expression::shared_ptr expression_;
};
}

#endif // UNARYEXP_H
