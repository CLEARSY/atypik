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
#ifndef SOLVERFACTORY_H
#define SOLVERFACTORY_H

#include "assertion.h"
#include "btypes.h"
#include "constraint.h"
#include "model.h"

#include <vector>

namespace solver
{

class ConstructorError : public std::exception
{
public:
    /*!
     * \brief Construct a ConstructorError from a message
     * \param msg
     * The message
     */
    ConstructorError(std::string msg);
    const char * what() const noexcept override;

private:
    std::string message_;
};

class Factory
{
public:
    /*!
     * \brief Create a shared pointer on an assertion from a contraint
     * \param constraint
     * The constraint
     * \return a shared pointer on the assertiion
     */
    Assertion::shared_ptr makeAssertion(AbstractConstraint::shared_ptr constraint);
    /*!
     * \brief Create a shared pointer on an assertion from a Equals constraint build
     * from two operands
     * \param left
     * The left side of the equality
     * \param right
     * The right side of the equality
     * \return a shared pointer on the assertion
     */
    Assertion::shared_ptr makeAssertEquals(AbstractTerm::shared_ptr left,
                                                  AbstractTerm::shared_ptr right);
    /*!
     * \brief Create a shared pointer on a B identifier
     * \param id
     * The identifier
     * \return a shared pointer on the B identifier
     */
    BIdent::shared_ptr makeBIdent(std::string id);
    /*!
     * \brief Create a shared pointer on a B identifier representing an integer
     * \return a shared pointer on the B identifier representing an integer
     */
    BIdent::shared_ptr makeInteger();
    /*!
     * \brief Create a shared pointer on a B identifier representing a real
     * \return a shared pointer on the B identifier representing a real
     */
    BIdent::shared_ptr makeReal();
    /*!
     * \brief Create a shared pointer on a B identifier representing a bool
     * \return a shared pointer on the B identifier representing a bool
     */
    BIdent::shared_ptr makeBool();
    /*!
     * \brief Create a shared pointer on a B identifier representing a string
     * \return a shared pointer on the B identifier representing a string
     */
    BIdent::shared_ptr makeString();
    /*!
     * \brief Create a shared pointer on a B cartesian product between two types
     * \param left
     * The left operand
     * \param right
     * The right operand
     * \return a shared pointer on the cartesian product between left and right
     */
    BCartesianProduct::shared_ptr makeBCartesianProduct(AbstractBType::shared_ptr left,
                                                              AbstractBType::shared_ptr right);
    /*!
     * \brief Create a shared pointer on B cartesian product between n types
     * \param types
     * A vector of n types
     * \return a shared pointer on the cartesian product between the n types
     */
    BCartesianProduct::shared_ptr makeBCartesianProduct(std::vector<AbstractBType::shared_ptr> types);
    /*!
     * \brief Create a shared pointer on a B POW
     * \param type
     * The type inside POW
     * \return a shared pointer on the POW
     */
    BPow::shared_ptr makeBPow(AbstractBType::shared_ptr type);
    /*!
     * \brief Create a shared pointer on a equality constraint
     * \param left
     * The left member of the equality
     * \param right
     * The right member of the equality
     * \return a shared pointer on the equality constraint
     */
    Equals::shared_ptr makeEquals(AbstractTerm::shared_ptr left,
                                         AbstractTerm::shared_ptr right);
    /*!
     * \brief Create a shared pointer on a conjunction constraint
     * \param left
     * The left member of the conjunction
     * \param right
     * The right member of the conjunction
     * \return a shared pointer on the conjunction constraint
     */
    Or::shared_ptr makeOr(AbstractTerm::shared_ptr left,
                                 AbstractTerm::shared_ptr right);
    /*!
     * \brief Create a shared pointer on a negation
     * \param term
     * The term to negate
     * \return a shared pointer on the negation constraint
     */
    Not::shared_ptr makeNot(AbstractTerm::shared_ptr term);
    /*!
     * \brief Create an empty model
     * \return a shared pointer on the model
     */
    Model::shared_ptr makeModel();
    /*!
     * \brief Create a variable from an identifier
     * \param id
     * The identifier
     * \return a shared pointer on the variable
     */
    static Variable::shared_ptr makeVariable(std::string id);

private:
    /*!
     * \brief A map storing all the identifiers already created by the factory to
     * avoid creating them again
     */
    std::unordered_map<std::string, BIdent::shared_ptr> identifers_;
    /*!
     * \brief A map storing all the power sets already created by the factory to
     * avoid creating them again
     */
    std::unordered_map<AbstractBType::shared_ptr, BPow::shared_ptr> power_sets_;
    /*!
     * \brief A map storing all the assertions already created by the factory to
     * avoid creating them again
     */
    std::unordered_map<std::string, Assertion::shared_ptr> assertions_;
};
}

#endif // SOLVERFACTORY_H
