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
#ifndef BTYPES_H
#define BTYPES_H

#include "abstractsolverelement.h"

namespace smt {
class AbsSmtSolver;
using SmtSolver = std::shared_ptr<AbsSmtSolver>;
class AbsTerm;
using Term = std::shared_ptr<AbsTerm>;
class AbsSort;
using Sort = std::shared_ptr<AbsSort>;
}

namespace solver
{

class AbstractBType : public AbstractTerm {
public:
    /*!
     * \brief A shared pointer on an AbstractBType
     */
    typedef std::shared_ptr<AbstractBType> shared_ptr;
    std::set<int> getVariables() override;
};

class BIdent : public AbstractBType
{
public:
    /*!
     * \brief Construct a B type from an identifier
     * \param id
     */
    BIdent(std::string id);
    std::string toSMT() override;
    smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) override;
    /*!
     * \brief A shared_ptr on a BIdent
     */
    typedef std::shared_ptr<BIdent> shared_ptr;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::string toString() override;

private:
    /*!
     * \brief The type id
     */
    std::string id_;
};

class BCartesianProduct : public AbstractBType
{
public:
    /*!
     * \brief Construct a cartesian product between two B types
     * \param left
     * The left operand
     * \param right
     * The right operand
     */
    BCartesianProduct(AbstractBType::shared_ptr left,
                      AbstractBType::shared_ptr right);
    std::string toSMT() override;
    smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::string toString() override;
    std::set<int> getVariables() override;
    /*!
     * \brief A shared_ptr on a BCartesianProduct
     */
    typedef std::shared_ptr<BCartesianProduct> shared_ptr;
    /*!
     * \brief An accessor on the left operand
     * \return the left operand
     */
    AbstractBType::shared_ptr getLeft();
    /*!
     * \brief An accessor on the right operand
     * \return the right operand
     */
    AbstractBType::shared_ptr getRight();

private:
    /*!
     * \brief The left operand
     */
    AbstractBType::shared_ptr left_;
    /*!
     * \brief The right operand
     */
    AbstractBType::shared_ptr right_;
};

class BPow : public AbstractBType
{
public:
    /*!
     * \brief Construct a B Pow type
     * \param type
     * The type inside the Pow
     */
    BPow(AbstractBType::shared_ptr type);
    std::string toSMT() override;
    smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::string toString() override;
    std::set<int> getVariables() override;
    /*!
     * \brief A shared_ptr on a BPow
     */
    typedef std::shared_ptr<BPow> shared_ptr;
    /*!
     * \brief An accessor on the type in the BPow
     * \return the type in the BPow
     */
    AbstractBType::shared_ptr getType();

private:
    /*!
     * \brief The type inside the POW
     */
    AbstractBType::shared_ptr type_;
};

class Variable : public AbstractBType
{
public:
    /*!
     * \brief Construct a variable from an identifier
     * \param id
     */
    Variable(std::string id);
    std::string toSMT() override;
    smt::Term getTerm(smt::SmtSolver solver, smt::Sort sort) override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::string toString() override;
    std::set<int> getVariables() override;
    /*!
     * \brief Provides a SMT declaration of the variable
     * \return the SMT declaration of the variable
     */
    std::string getSMTDeclaration();
    /*!
     * \brief A shared_ptr on a Variable
     */
    typedef std::shared_ptr<Variable> shared_ptr;
    /*!
     * \brief An accessor on the numeric id of the variable
     * \return
     */
    int getNumericId();

private:
    /*!
     * \brief The type id
     */
    std::string id_;
    /*!
     * \brief The unique numeric id of the variable
     */
    int numeric_id_;
    /*!
     * \brief A counter on the occurences of variables
     */
    int static cpt_;
};
}

#endif // BTYPES_H
