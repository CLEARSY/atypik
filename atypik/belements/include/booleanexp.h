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
#ifndef BOOLEANEXP_H
#define BOOLEANEXP_H

#include "expression.h"
#include "predicate.h"

namespace belem
{
class BooleanExpression : public Expression
{
public:
    /*!
     * \brief Construct an object representing the conversion of a predicate
     * into a boolean
     * \param pos
     * The position of the boolean expression
     */
    BooleanExpression(Predicate::shared_ptr,
                      bxml::Position::shared_ptr pos);
    std::string format() override;

private:
    /*!
     * \brief The predicate to convert
     */
    Predicate::shared_ptr predicate_;
};
}

#endif // BOOLEANEXP_H
