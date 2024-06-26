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
#ifndef IDENT_H
#define IDENT_H

#include "expression.h"

namespace belem
{
class Ident : public Expression
{
public:
    /*!
     * \brief Construct a B identifier that can represents a constant or
     * a variable
     * \param value
     * The value of the identifier
     * \param position
     * The position of the identifier
     */
    Ident(std::string value,
          bxml::Position::shared_ptr position);
    std::string format() override;

private:
    /*!
     * \brief The value of the identifier
     */
    std::string id_;
};
}

#endif // IDENT_H
