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
#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "abstractsolverelement.h"
#include "btypes.h"

#include <memory>
#include <string>

namespace solver
{
class AbstractConstraint : public AbstractTerm
{
public:
    typedef std::shared_ptr<AbstractConstraint> shared_ptr;
    /*!
     * \brief Initialize the id of a constraint
     */
    AbstractConstraint();
    /*!
     * \brief An accessor on the id of the constraint
     * \return the id of the constraint
     */
    std::string getId();

private:
    /*!
     * \brief The id of the assertion
     */
    std::string id_;
    /*!
     * \brief A counter representing the number of assertions generated
     */
    static unsigned int cpt_;
};

class Equals : public AbstractConstraint
{
public:
    /*!
     * \brief Construct an equality constraint between two solver elements
     * \param left
     * The left member of the constraint
     * \param right
     * The right member of the constraint
     */
    Equals(AbstractTerm::shared_ptr left,
           AbstractTerm::shared_ptr right);
    /*!
     * \brief A shared pointer on Equals
     */
    typedef std::shared_ptr<Equals> shared_ptr;
    std::string toSMT() override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::set<int> getVariables() override;
    smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) override;
    std::string toString() override;

private:
    /*!
     * \brief The left member of the constraint
     */
    AbstractTerm::shared_ptr left_;
    /*!
     * \brief The right member of the constraint
     */
    AbstractTerm::shared_ptr right_;

};

class Or : public AbstractConstraint {
public:
    /*!
     * \brief Construct a conjunction constraint between two solver elements
     * \param left
     * The left member of the conjunction
     * \param right
     * The right member of the conjunction
     */
    Or(AbstractTerm::shared_ptr left,
       AbstractTerm::shared_ptr right);
    /*!
     * \brief A shared pointer on a Or
     */
    typedef std::shared_ptr<Or> shared_ptr;
    std::string toSMT() override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::set<int> getVariables() override;
    smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) override;
    std::string toString() override;

private:
    /*!
     * \brief The left member of the constraint
     */
    AbstractTerm::shared_ptr left_;
    /*!
     * \brief The right member of the constraint
     */
    AbstractTerm::shared_ptr right_;
};

class Not : public AbstractConstraint {
public:
    /*!
     * \brief Construct the negation of a term
     * \param term
     * The term
     */
    Not(AbstractTerm::shared_ptr term);
    /*!
     * \brief A shared pointer on a Not
     */
    typedef std::shared_ptr<Not> shared_ptr;
    std::string toSMT() override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::set<int> getVariables() override;
    smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) override;
    std::string toString() override;


private:
    /*!
     * \brief The term
     */
    AbstractTerm::shared_ptr term_;
};
}

#endif // CONSTRAINT_H
