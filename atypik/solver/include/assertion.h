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
#ifndef ASSERTION_H
#define ASSERTION_H

#include "abstractsolverelement.h"
#include "constraint.h"

namespace solver
{
class Assertion : public AbstractSolverElement
{
public:
    /*!
     * \brief Construct an assertion on a constraint
     * \param constraint
     * The constraint
     */
    Assertion(AbstractConstraint::shared_ptr constraint);
    /*!
     * \brief A shared pointer on an Assertion
     */
    typedef std::shared_ptr<Assertion> shared_ptr;
    std::string toSMT() override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::set<int> getVariables() override;
    /*!
     * \brief An accessor on the constraint
     * \return the constraint
     */
    AbstractConstraint::shared_ptr getConstraint();

private:
    /*!
     * \brief The constraint on which the assertion is done
     */
    AbstractConstraint::shared_ptr constraint_;
};
}

#endif // ASSERTION_H
