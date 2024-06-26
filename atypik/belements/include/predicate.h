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
#ifndef PREDICATE_H
#define PREDICATE_H

#include "belement.h"

#include <vector>

namespace belem
{

class Predicate : public AbstractBElement {};

class NaryPred : public Predicate
{
public:
    /*!
     * \brief Construct a n-arity predicate from a vector of clauses and an operator
     * \param clauses
     * The clauses of the predicate
     * \param op
     * The operator
     */
    NaryPred(std::vector<Predicate::shared_ptr> clauses, std::string op);
    std::string format() override;

private:
    /*!
     * \brief The clauses in the predicate
     */
    std::vector<Predicate::shared_ptr> clauses_;
    /*!
     * \brief The operator
     */
    std::string op_;
};

class UnaryPred : public Predicate
{
public:
    /*!
     * \brief Construct a predicate from an operator and another predicate
     * \param op
     * The operator
     * \param predicate
     * The predicate
     */
    UnaryPred(std::string op, Predicate::shared_ptr predicate);
    std::string format() override;

private:
    /*!
     * \brief The operator
     */
    std::string op_;
    /*!
     * \brief The predicate
     */
    Predicate::shared_ptr predicate_;
};
}

#endif // PREDICATE_H
