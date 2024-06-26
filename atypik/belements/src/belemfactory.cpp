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
#include "../include/belemfactory.h"

using std::make_shared;
using std::string;
using std::vector;

namespace belem {

// Expressions

BinaryExp::shared_ptr Factory::makeBinaryExp(
    Expression::shared_ptr left, string op, Expression::shared_ptr right,
    bxml::Position::shared_ptr position) {
  return make_shared<BinaryExp>(left, op, right, position);
}

BooleanExpression::shared_ptr Factory::makeBooleanExp(
    Predicate::shared_ptr predicate, bxml::Position::shared_ptr position) {
  return make_shared<BooleanExpression>(predicate, position);
}

BBool::shared_ptr Factory::makeBool(std::string value,
                                    bxml::Position::shared_ptr position) {
  return make_shared<BBool>(value, position);
}

BInteger::shared_ptr Factory::makeInt(string value,
                                      bxml::Position::shared_ptr position) {
  return make_shared<BInteger>(value, position);
}

BReal::shared_ptr Factory::makeReal(string value,
                                    bxml::Position::shared_ptr position) {
  return make_shared<BReal>(value, position);
}

BString::shared_ptr Factory::makeString(string value,
                                        bxml::Position::shared_ptr position) {
  return make_shared<BString>(value, position);
}

Ident::shared_ptr Factory::makeIdent(std::string value,
                                     bxml::Position::shared_ptr position) {
  return make_shared<Ident>(value, position);
}

Set::shared_ptr Factory::makeSet(Ident::shared_ptr id,
                                 vector<Expression::shared_ptr> content,
                                 bxml::Position::shared_ptr position) {
  return make_shared<Set>(id, content, position);
}

UnaryExp::shared_ptr Factory::makeUnaryExp(
    string op, Expression::shared_ptr expression,
    bxml::Position::shared_ptr position) {
  return make_shared<UnaryExp>(op, expression, position);
}

NaryExp::shared_ptr Factory::makeNaryExp(
    vector<Expression::shared_ptr> operands, string op,
    bxml::Position::shared_ptr position) {
  return make_shared<NaryExp>(operands, op, position);
}

QuantifiedExp::shared_ptr Factory::makeQuantifiedExp(
    string type, vector<Expression::shared_ptr> identifiers,
    Predicate::shared_ptr predicate, Expression::shared_ptr expression,
    bxml::Position::shared_ptr position) {
  return make_shared<QuantifiedExp>(type, identifiers, predicate, expression,
                                    position);
}

QuantifiedSet::shared_ptr Factory::makeQuantifiedSet(
    vector<Expression::shared_ptr> expression, Predicate::shared_ptr predicate,
    bxml::Position::shared_ptr position) {
  return make_shared<QuantifiedSet>(expression, predicate, position);
}

Valuation::shared_ptr Factory::makeValuation(
    Expression::shared_ptr variable, Expression::shared_ptr value,
    bxml::Position::shared_ptr position) {
  return make_shared<Valuation>(variable, value, position);
}

// Predicates

Comparison::shared_ptr Factory::makeComparison(Expression::shared_ptr left,
                                               string op,
                                               Expression::shared_ptr right) {
  return make_shared<Comparison>(left, op, right);
}

NaryPred::shared_ptr Factory::makeNaryPred(
    vector<Predicate::shared_ptr> clauses, string op) {
  return make_shared<NaryPred>(clauses, op);
}

UnaryPred::shared_ptr Factory::makeUnaryPred(string op,
                                             Predicate::shared_ptr predicate) {
  return make_shared<UnaryPred>(op, predicate);
}

QuantifiedPred::shared_ptr Factory::makeQuantifiedPred(
    string op, vector<Expression::shared_ptr> variables,
    Predicate::shared_ptr body) {
  return make_shared<QuantifiedPred>(op, variables, body);
}

BinaryPred::shared_ptr Factory::makeBinaryPred(Predicate::shared_ptr left,
                                               string op,
                                               Predicate::shared_ptr right) {
  return make_shared<BinaryPred>(left, op, right);
}

// Operation

Operation::shared_ptr Factory::makeOperation(
    std::string name, std::vector<Ident::shared_ptr> input_params,
    std::vector<Ident::shared_ptr> output_params,
    Predicate::shared_ptr precondition, Instruction::shared_ptr body,
    solver::Variable::shared_ptr var) {
  return make_shared<Operation>(name, input_params, output_params, precondition,
                                body, var);
}

// Instructions

Assignments::shared_ptr Factory::makeAssignments(
    vector<Expression::shared_ptr> variables,
    vector<Expression::shared_ptr> values) {
  return make_shared<Assignments>(variables, values);
}

NarySub::shared_ptr Factory::makeNarySub(
    vector<Instruction::shared_ptr> instructions, string op) {
  return make_shared<NarySub>(instructions, op);
}

Block::shared_ptr Factory::makeBlock(Instruction::shared_ptr body) {
  return make_shared<Block>(body);
}

Skip::shared_ptr Factory::makeSkip() { return make_shared<Skip>(); }

Assert::shared_ptr Factory::makeAssert(Predicate::shared_ptr condition,
                                       Instruction::shared_ptr body) {
  return make_shared<Assert>(condition, body);
}

Conditional::shared_ptr Factory::makeIf(Predicate::shared_ptr condition,
                                        Instruction::shared_ptr consequent,
                                        Instruction::shared_ptr alternative) {
  return make_shared<Conditional>(condition, consequent, alternative);
}

Select::shared_ptr Factory::makeSelect(
    vector<Predicate::shared_ptr> conditions,
    vector<Instruction::shared_ptr> consequences,
    Instruction::shared_ptr alternative) {
  return make_shared<Select>(conditions, consequences, alternative);
}

Case::shared_ptr Factory::makeCase(Expression::shared_ptr value,
                                   vector<Expression::shared_ptr> choices,
                                   vector<Instruction::shared_ptr> consequences,
                                   Instruction::shared_ptr alternative) {
  return make_shared<Case>(value, choices, consequences, alternative);
}

Any::shared_ptr Factory::makeAny(vector<Expression::shared_ptr> variables,
                                 Predicate::shared_ptr predicate,
                                 Instruction::shared_ptr body) {
  return make_shared<Any>(variables, predicate, body);
}

Let::shared_ptr Factory::makeLet(vector<Expression::shared_ptr> variables,
                                 vector<Expression::shared_ptr> values,
                                 Instruction::shared_ptr body) {
  return make_shared<Let>(variables, values, body);
}

BecomesIn::shared_ptr Factory::makeBecomesIn(
    vector<Expression::shared_ptr> variables, Expression::shared_ptr set) {
  return make_shared<BecomesIn>(variables, set);
}

BecomesSuchThat::shared_ptr Factory::makeBecomesSuchThat(
    vector<Expression::shared_ptr> variables, Predicate::shared_ptr predicate) {
  return make_shared<BecomesSuchThat>(variables, predicate);
}

VarIn::shared_ptr Factory::makeVarIn(vector<Expression::shared_ptr> variables,
                                     Instruction::shared_ptr body) {
  return make_shared<VarIn>(variables, body);
}

OperationCall::shared_ptr Factory::makeOperationCall(
    string name, vector<Expression::shared_ptr> inputs,
    vector<Expression::shared_ptr> outputs) {
  return make_shared<OperationCall>(name, inputs, outputs);
}

Loop::shared_ptr Factory::makeLoop(Predicate::shared_ptr condition,
                                   Instruction::shared_ptr body,
                                   Predicate::shared_ptr invariant,
                                   Expression::shared_ptr variant) {
  return make_shared<Loop>(condition, body, invariant, variant);
}

}  // namespace belem
