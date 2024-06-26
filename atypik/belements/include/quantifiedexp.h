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
#ifndef QUANTIFIEDEXP_H
#define QUANTIFIEDEXP_H

#include "expression.h"
#include "predicate.h"

#include <vector>

namespace belem
{
class QuantifiedExp : public Expression
{
public:
    /*!
     * \brief Create a B quantified expression
     * \param type
     * The type of the expression
     * \param identifiers
     * A vector of identifiers
     * \param predicate
     * The predicate
     * \param expression
     * The expression
     * \param position
     * The position in a bxml file
     */
    QuantifiedExp(std::string type,
                  std::vector<Expression::shared_ptr> identifiers,
                  Predicate::shared_ptr predicate,
                  Expression::shared_ptr expression,
                  bxml::Position::shared_ptr position);
    std::string format() override;

private:
    /*!
     * \brief The type
     */
    std::string type_;
    /*!
     * \brief The identifiers
     */
    std::vector<Expression::shared_ptr> identifiers_;
    /*!
     * \brief The predicate
     */
    Predicate::shared_ptr predicate_;
    /*!
     * \brief The expression
     */
    Expression::shared_ptr expression_;
};
}

#endif // QUANTIFIEDEXP_H
