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
#ifndef QUANTIFIEDSET_H
#define QUANTIFIEDSET_H

#include "expression.h"
#include "predicate.h"

#include <vector>

namespace belem
{
class QuantifiedSet : public Expression
{
public:
    /*!
     * \brief Construct a B quantified set from a vector of expressions and a predicate
     * \param expressions
     * The expressions
     * \param predicate
     * The predicate
     * \param position
     * The position
     */
    QuantifiedSet(std::vector<Expression::shared_ptr> expressions,
                  Predicate::shared_ptr predicate,
                  bxml::Position::shared_ptr position);
    std::string format() override;

private:
    /*!
     * \brief The expressions in the quantified set
     */
    std::vector<Expression::shared_ptr> expressions_;
    /*!
     * \brief The predicate in the quantified set
     */
    Predicate::shared_ptr predicate_;
};
}

#endif // QUANTIFIEDSET_H
