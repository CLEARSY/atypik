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
#ifndef BELEMENT_H
#define BELEMENT_H

#include <memory>
#include <string>

namespace belem
{
/*!
 * \brief The AbstractBElement class represents a B element (an instruction or
 * an expression)
 */
class AbstractBElement
{
public:
    /*!
     * \brief A shared_ptr on an AbstractBElement
     */
    typedef std::shared_ptr<AbstractBElement> shared_ptr;
    /*!
     * \brief Provides a string representation of a B expression
     * \return the representation of the B expression
     */
    virtual std::string format() = 0;
};

}

#endif // BELEMENT_H
