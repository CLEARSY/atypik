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
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <set>

#include "belement.h"
#include "bxmlposition.h"
#include "btypes.h"
#include "vargen.h"

namespace belem
{
/*!
 * \brief The Expression class represents an expression in B
 */
class Expression : public AbstractBElement
{
public:
    /*!
     * \brief Associate a variable to the expression
     * \param positions
     * The positions of the expression in a bxml file
     *
     */
    Expression(std::set<bxml::Position::shared_ptr> positions);
    /*!
     * \brief A shared_ptr on an Expression
     */
    typedef std::shared_ptr<Expression> shared_ptr;
    /*!
     * \brief An accessor on the variable representing the type of
     * the expression
     * \return a shared pointer on the variable representing the type
     * of the expression
     */
    virtual solver::Variable::shared_ptr getAssociatedVariable()
    {
        return var_;
    };
    /*!
     * \brief An accessor on the positions of the expression
     * \return the positions of the expression
     */
    virtual std::set<bxml::Position::shared_ptr> getPositions()
    {
        return positions_;
    }
    /*!
     * \brief Add a position to the expression
     * \param position
     * The position to add
     */
    virtual void addPosition(bxml::Position::shared_ptr position)
    {
        positions_.insert(position);
    }
    /*!
     * \brief Clear the position of an expression
     */
    virtual void clearPositions(){
        positions_.clear();
    }

private:
    /*!
     * \brief The associated type variable of the expression
     */
    solver::Variable::shared_ptr var_;
    /*!
     * \brief The positions of the variables in a bxml file
     */
    std::set<bxml::Position::shared_ptr> positions_;
};
}

#endif // EXPRESSION_H
