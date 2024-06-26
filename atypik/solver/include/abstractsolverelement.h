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
#ifndef ABSTRACTSOLVERELEMENT_H
#define ABSTRACTSOLVERELEMENT_H

#include <memory>
#include <set>
#include <string>

namespace smt {
class AbsTerm;
using Term = std::shared_ptr<AbsTerm>;
class AbsSmtSolver;
using SmtSolver = std::shared_ptr<AbsSmtSolver>;
class AbsSort;
using Sort = std::shared_ptr<AbsSort>;
}

namespace solver
{
/*!
 * \brief The AbstractSolverElement class represents a variable, a constant
 * or a constraint
 */
class AbstractSolverElement
{
public:
    /*!
     * \brief A shared pointer on an abstract solver element
     */
    typedef std::shared_ptr<AbstractSolverElement> shared_ptr;
    /*!
     * \brief Compute a representation of the element following the smt format
     * \return the smt representation of the element
     */
    virtual std::string toSMT() = 0;
    /*!
     * \brief Check if a variable is used by the solver element
     * \param var
     * The variable
     * \return true if the variable is used by the solver, false otherwise
     */
    virtual bool contains(AbstractSolverElement::shared_ptr var) = 0;
    /*!
     * \brief An accessor on the identifiers of the variables in the solver element
     * \return the identifiers of the variables in the solver element
     */
    virtual std::set<int> getVariables() = 0;
};

/*!
 * \brief The AbstractTerm class represents a solver element which can be used as a
 * Term
 */
class AbstractTerm : public AbstractSolverElement {
public:
    /*!
     * \brief Create a term from a solver corresponding to the type
     * \param solver
     * The solver
     * \param sort
     * The sort
     * \return the created term
     */
    virtual smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) = 0;
    /*!
     * \brief A shared pointer on an abstract term
     */
    typedef std::shared_ptr<AbstractTerm> shared_ptr;
    /*!
     * \brief Provides a string representation of a solver element
     * \return a string representation of a constraint without its name
     */
    virtual std::string toString() = 0;
};

}

#endif // ABSTRACTSOLVERELEMENT_H
