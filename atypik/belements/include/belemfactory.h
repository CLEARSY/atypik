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
#ifndef BELEMFACTORY_H
#define BELEMFACTORY_H

#include "any.h"
#include "assert.h"
#include "assignments.h"
#include "bdefaulttypes.h"
#include "becomesin.h"
#include "becomessuchthat.h"
#include "binaryexp.h"
#include "binarypred.h"
#include "block.h"
#include "booleanexp.h"
#include "case.h"
#include "comparison.h"
#include "conditional.h"
#include "ident.h"
#include "let.h"
#include "loop.h"
#include "naryexp.h"
#include "narysub.h"
#include "operation.h"
#include "operationcall.h"
#include "predicate.h"
#include "quantifiedexp.h"
#include "quantifiedpred.h"
#include "quantifiedset.h"
#include "select.h"
#include "set.h"
#include "skip.h"
#include "unaryexp.h"
#include "valuation.h"
#include "varin.h"

namespace belem
{
class Factory
{
public:
    // Expressions
    /*!
     * \brief Create a B binary expression
     * \param left
     * The left operand
     * \param op
     * The operator
     * \param right
     * The right operand
     * \param position
     * The position of the binary expression
     * \return a pointer on the binary expression
     */
    static BinaryExp::shared_ptr makeBinaryExp(Expression::shared_ptr left,
                                               std::string op,
                                               Expression::shared_ptr right,
                                               bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B boolean
     * \param value
     * The value of the boolean
     * \param position
     * The position of the boolean
     * \return a pointer on the boolean
     */
    static BBool::shared_ptr makeBool(std::string value,
                                      bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B boolean expression
     * \param predicate
     * The predicate to cast
     * \param position
     * The position of the boolean expression
     * \return a pointer on the boolean expression
     */
    static BooleanExpression::shared_ptr makeBooleanExp(Predicate::shared_ptr predicate,
                                                        bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B integer
     * \param value
     * The value of the integer
     * \param position
     * The position of the integer
     * \return a pointer on the integer
     */
    static BInteger::shared_ptr makeInt(std::string value,
                                        bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B real
     * \param value
     * The value of the real
     * \param position
     * The position of the real
     * \return a pointer on the real
     */
    static BReal::shared_ptr makeReal(std::string value,
                                      bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B string
     * \param value
     * The value of the string
     * \param position
     * The position of the string
     * \return a pointer on the string
     */
    static BString::shared_ptr makeString(std::string value,
                                          bxml::Position::shared_ptr position);
    /*!
     * \brief Create an B identifier
     * \param value
     * The value of the identifier
     * \param position
     * The position of the identifier
     * \return a pointer on an Ident
     */
    static Ident::shared_ptr makeIdent(std::string value,
                                       bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B set
     * \param id
     * The identifier of the set
     * \param content
     * The content of the set
     * \param position
     * The position of the identifier
     * \return a pointer on a Set
     */
    static Set::shared_ptr makeSet(Ident::shared_ptr id,
                                   std::vector<Expression::shared_ptr> content,
                                   bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B unary expression
     * \param op
     * The operator
     * \param expression
     * The argument
     * \param position
     * The position of the identifier
     * \return a pointer on an UnaryExp
     */
    static UnaryExp::shared_ptr makeUnaryExp(std::string op,
                                             Expression::shared_ptr expression,
                                             bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B n-arity expression
     * \param operands
     * A vector of n operands
     * \param op
     * The operator
     * \param position
     * The position of the n-arity expression
     * \return a pointer on a NaryExp
     */
    static NaryExp::shared_ptr makeNaryExp(std::vector<Expression::shared_ptr> operands,
                                           std::string op,
                                           bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B quantified expression
     * \param type
     * The type of the expression
     * \param identifiers
     * The identifiers introduced by the expression
     * \param predicate
     * The predicate in the expression
     * \param expression
     * The quantified expression
     * \param position
     * The position of the expression
     * \return a pointer on a QuantifiedExp
     */
    static QuantifiedExp::shared_ptr makeQuantifiedExp(std::string type,
                                                       std::vector<Expression::shared_ptr> identifiers,
                                                       Predicate::shared_ptr predicate,
                                                       Expression::shared_ptr expression,
                                                       bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B quantified set
     * \param expressions
     * The expressions in the set
     * \param predicate
     * The predicate in the set
     * \param position
     * The position of the set
     * \return a pointer on a QuantifiedSet
     */
    static QuantifiedSet::shared_ptr makeQuantifiedSet(std::vector<Expression::shared_ptr> expression,
                                                       Predicate::shared_ptr predicate,
                                                       bxml::Position::shared_ptr position);
    /*!
     * \brief Create a B valuation
     * \param variable
     * The variable to valuate
     * \param value
     * The value binded to the variable
     * \param position
     * The position of the valuation
     * \return a pointer on a Valuation
     */
    static Valuation::shared_ptr makeValuation(Expression::shared_ptr variable,
                                               Expression::shared_ptr value,
                                               bxml::Position::shared_ptr position);

    // Predicates
    /*!
     * \brief Create a comparison between two expressions
     * \param left
     * The left operand
     * \param op
     * The operator
     * \param right
     * The right operand
     * \return a pointer on a Comparison
     */
    static Comparison::shared_ptr makeComparison(Expression::shared_ptr left,
                                                 std::string op,
                                                 Expression::shared_ptr right);
    /*!
     * \brief Create a n-arity predicate
     * \param clauses
     * The clauses of the predicate
     * \param op
     * The operator
     * \return a pointer on a NaryPred
     */
    static NaryPred::shared_ptr makeNaryPred(std::vector<Predicate::shared_ptr> clauses,
                                             std::string op);
    /*!
     * \brief Create a unary predicate
     * \param op
     * The operator
     * \param predicate
     * The predicate
     * \return a pointer on a UnaryPred
     */
    static UnaryPred::shared_ptr makeUnaryPred(std::string op, Predicate::shared_ptr predicate);
    /*!
     * \brief Create a quantified predicate
     * \param op
     * The operator
     * \param variables
     * The variables
     * \param body
     * The body
     * \return a pointer on a QuantifiedPred
     */
    static QuantifiedPred::shared_ptr makeQuantifiedPred(std::string op,
                                                         std::vector<Expression::shared_ptr> variables,
                                                         Instruction::shared_ptr body);
    /*!
     * \brief Create a binary predicate
     * \param left
     * The left operand
     * \param op
     * The operator
     * \param right
     * The right operand
     * \return a pointer on a BinaryPred
     */
    static BinaryPred::shared_ptr makeBinaryPred(Predicate::shared_ptr left,
                                                 std::string op,
                                                 Predicate::shared_ptr right);
    // Operation
    /*!
     * \brief Create an operation
     * \param name
     * The name of the operation
     * \param input_params
     * The input parameters of the operation
     * \param output_params
     * The output parameters of the operation
     * \param precondition
     * The precondition of the operation
     * \param body
     * The body of the operation
     * \param var
     * The variable associated to the type of the operation
     * \return a pointer on an Operation
     */
    static Operation::shared_ptr makeOperation(std::string name,
                                               std::vector<Ident::shared_ptr> input_params,
                                               std::vector<Ident::shared_ptr> output_params,
                                               Predicate::shared_ptr precondition,
                                               Instruction::shared_ptr body,
                                               solver::Variable::shared_ptr var);
    // Instructions

    /*!
     * \brief Create B assignments
     * \param variables
     * The variables to change
     * \param values
     * The values to assign
     * \return a pointer on an Assignments
     */
    static Assignments::shared_ptr makeAssignments(std::vector<Expression::shared_ptr> variables,
                                                   std::vector<Expression::shared_ptr> values);
    /*!
     * \brief Create a substitution of arity n
     * \param instructions
     * The n instructions
     * \param op
     * The operator between the instructions
     * \return a pointer on a NarySub
     */
    static NarySub::shared_ptr makeNarySub(std::vector<Instruction::shared_ptr> instructions,
                                           std::string op);
    /*!
     * \brief Create a block
     * \param body
     * The body of the block
     * \return a pointer on a Block
     */
    static Block::shared_ptr makeBlock(Instruction::shared_ptr body);
    /*!
     * \brief Create a skip instruction
     * \return a pointer on a Skip
     */
    static Skip::shared_ptr makeSkip();
    /*!
     * \brief Create an assert instruction
     * \param condition
     * The condition of the assertion
     * \param body
     * The body of the assertion
     * \return a pointer on an Assertion
     */
    static Assert::shared_ptr makeAssert(Predicate::shared_ptr condition,
                                         Instruction::shared_ptr body);
    /*!
     * \brief Create a conditional instruction
     * \param condition
     * The condition
     * \param consequent
     * The consequent
     * \param alternative
     * The alternative
     * \return a pointer on a Conditional
     */
    static Conditional::shared_ptr makeIf(Predicate::shared_ptr condition,
                                          Instruction::shared_ptr consequent,
                                          Instruction::shared_ptr alternative);
    /*!
     * \brief Create a select instruction
     * \param conditions
     * The conditions in the select
     * \param consequences
     * The consequences of the conditions
     * \param alternative
     * The alternative
     * \return a pointer on a Select
     */
    static Select::shared_ptr makeSelect(std::vector<Predicate::shared_ptr> conditions,
                                         std::vector<Instruction::shared_ptr> consequences,
                                         Instruction::shared_ptr alternative);
    /*!
     * \brief Create a case instruction
     * \param value
     * The value
     * \param choices
     * The choices
     * \param consequences
     * The consequences
     * \param alternative
     * The alternative
     * \return a pointer on a Case
     */
    static Case::shared_ptr makeCase(Expression::shared_ptr value,
                                     std::vector<Expression::shared_ptr> choices,
                                     std::vector<Instruction::shared_ptr> consequences,
                                     Instruction::shared_ptr alternative);
    /*!
     * \brief Create an any instruction
     * \param variables
     * The variables introduced by the instruction
     * \param predicate
     * The predicate of the instruction
     * \param body
     * The body of the instruction
     * \return a pointer on an Any
     */
    static Any::shared_ptr makeAny(std::vector<Expression::shared_ptr> variables,
                                   Predicate::shared_ptr predicate,
                                   Instruction::shared_ptr body);
    /*!
     * \brief Create a let instruction
     * \param variables
     * The variables introduced by the instruction
     * \param values
     * The values of the introduced variables
     * \param body
     * The body of the instruction
     * \return a pointer on the Let
     */
    static Let::shared_ptr makeLet(std::vector<Expression::shared_ptr> variables,
                                   std::vector<Expression::shared_ptr> values,
                                   Instruction::shared_ptr body);
    /*!
     * \brief Create a becomes in instruction
     * \param variables
     * The variables
     * \param set
     * The set containing the identifiers
     * \return a pointer on the BecomesIn
     */
    static BecomesIn::shared_ptr makeBecomesIn(std::vector<Expression::shared_ptr> variables,
                                               Expression::shared_ptr set);
    /*!
     * \brief Create a becomes such that instruction
     * \param variables
     * The variables
     * \param predicate
     * The predicate
     * \return a pointer on the BecomesSuchThat
     */
    static BecomesSuchThat::shared_ptr makeBecomesSuchThat(std::vector<Expression::shared_ptr> variables,
                                                           Predicate::shared_ptr predicate);
    /*!
     * \brief Create a var in instruction
     * \param variables
     * The variables introduced by the instruction
     * \param body
     * The body of the instruction
     * \return a pointer on the VarIn
     */
    static VarIn::shared_ptr makeVarIn(std::vector<Expression::shared_ptr> variables,
                                       Instruction::shared_ptr body);
    /*!
     * \brief Create an operation call instruction
     * \param name
     * The name of the operation to call
     * \param inputs
     * The input arguments
     * \param outputs
     * The output arguments
     * \return a pointer on the OperationCall
     */
    static OperationCall::shared_ptr makeOperationCall(std::string name,
                                                       std::vector<Expression::shared_ptr> inputs,
                                                       std::vector<Expression::shared_ptr> outputs);
    /*!
     * \brief Create a while instruction
     * \param condition
     * The condition of the loop
     * \param body
     * The body of the loop
     * \param invariant
     * The invariant of the loop
     * \param variant
     * The variant of the loop
     * \return a pointer on the Loop
     */
    static Loop::shared_ptr makeLoop(Predicate::shared_ptr condition,
                                     Instruction::shared_ptr body,
                                     Predicate::shared_ptr invariant,
                                     Expression::shared_ptr variant);
};
}

#endif // BELEMFACTORY_H
