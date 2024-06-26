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
#ifndef SET_H
#define SET_H

#include "ident.h"

#include <vector>

namespace belem
{
class Set : public Expression
{
public:
    /*!
     * \brief Construct a B set
     * \param id
     * The identifier of the set
     * \param content
     * The content of the set
     * \param var
     * The var associated to the type of the set
     * \param position
     * The position of the set in the bxml file
     */
    Set(Ident::shared_ptr id,
        std::vector<Expression::shared_ptr> content,
        bxml::Position::shared_ptr position);
    std::string format() override;
    /*!
     * \brief A shared_ptr on a Set
     */
    typedef std::shared_ptr<Set> shared_ptr;
    /*!
     * \brief An accessor on the identifier of the set
     * \return the identifier of the set
     */
    Ident::shared_ptr getID();
    /*!
     * \brief Check if the set is an enumeration
     * \return true if the set is an enumeration, false otherwise
     */
    bool isAnEnumeration();
    /*!
     * \brief An accessor on the enumerated values of the set
     * \return the enumerated values of the set if it is an enumeration,
     * an empty vector otherwise
     */
    const std::vector<Expression::shared_ptr>& getEnumeratedValues();

private:
    /*!
     * \brief The identifier of the set
     */
    Ident::shared_ptr id_;
    /*!
     * \brief The content of the set
     */
    std::vector<Expression::shared_ptr> content_;
};
}

#endif // SET_H
