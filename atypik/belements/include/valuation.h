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
#ifndef VALUATION_H
#define VALUATION_H

#include "expression.h"

namespace belem {
class Valuation : public Expression {
public:
    /*!
     * \brief Construct a B valuation
     * \param variable
     * The variable to valuate
     * \param value
     * The value binded to the variable
     */
    Valuation(Expression::shared_ptr variable,
              Expression::shared_ptr value,
              bxml::Position::shared_ptr position);
    std::string format() override;

private:
    /*!
     * \brief The variable to valuate
     */
    Expression::shared_ptr variable_;
    /*!
     * \brief The value binded to the variable
     */
    Expression::shared_ptr value_;
};
}

#endif // VALUATION_H
