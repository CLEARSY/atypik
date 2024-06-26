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
#ifndef BXMLPOSITION_H
#define BXMLPOSITION_H

#include "tinyxml2.h"

#include <memory>

namespace bxml
{
class Position
{
public:
    /*!
     * \brief Construct a position from a line, a column and a span
     * \param line
     * The line
     * \param column
     * The column
     * \param span
     * The span
     * \param tag
     * The tag of the expression in this position
     */
    Position(int line, int column, int span, tinyxml2::XMLElement *pElement);
    /*!
     * \brief A shared_ptr on a position
     */
    typedef std::shared_ptr<Position> shared_ptr;
    /*!
     * \brief An accessor on the line
     * \return the line
     */
    int getLine();
    /*!
     * \brief An accessor on the column
     * \return the column
     */
    int getColumn();
    /*!
     * \brief An accessor on the span
     * \return the span
     */
    int getSpan();
    /*!
     * \brief An accessor on the tag
     * \return the tag
     */
    std::string getTag();
    /*!
     * \brief An accessor on the tinyxml2 element linked to the position
     * \return the tinyxml2 element
     */
    tinyxml2::XMLElement * getTinyXMLElement();

private:
    /*!
     * \brief The line
     */
    int line_;
    /*!
     * \brief The column
     */
    int column_;
    /*!
     * \brief The span
     */
    int span_;
    /*!
     * \brief The tag
     */
    std::string tag_;
    /*!
     * \brief A pointer on the element in the file
     */
    tinyxml2::XMLElement *pElement_;
};
}

#endif // BXMLPOSITION_H
