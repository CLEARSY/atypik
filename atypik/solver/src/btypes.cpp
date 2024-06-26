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
#include "btypes.h"

#include "smt.h"

using namespace smt;
using std::set;
using std::string;

namespace solver {

// Implementation of the AbstractBType class

set<int> AbstractBType::getVariables() { return {}; }

// Implementation of the BIdent class

BIdent::BIdent(string id) : id_(id) {}

string BIdent::toSMT() { return id_; }

Term BIdent::getTerm(SmtSolver solver, Sort sort) {
  Term ident = solver->get_constructor(sort, id_);
  return solver->make_term(Apply_Constructor, ident);
}

bool BIdent::contains(AbstractSolverElement::shared_ptr var) { return false; }

string BIdent::toString() { return id_; }

// Implementation of the BCartesianProduct class

BCartesianProduct::BCartesianProduct(AbstractBType::shared_ptr left,
                                     AbstractBType::shared_ptr right)
    : left_(left), right_(right) {}

string BCartesianProduct::toSMT() {
  return "(PRODUCT " + left_->toSMT() + " " + right_->toSMT() + ")";
}

Term BCartesianProduct::getTerm(SmtSolver solver, Sort sort) {
  Term left_term = left_->getTerm(solver, sort);
  Term right_term = right_->getTerm(solver, sort);
  Term product = solver->get_constructor(sort, "PRODUCT");
  return solver->make_term(Apply_Constructor, product, left_term, right_term);
}

bool BCartesianProduct::contains(AbstractSolverElement::shared_ptr var) {
  return left_->contains(var) or right_->contains(var);
}

string BCartesianProduct::toString() {
  return left_->toString() + " x " + right_->toString();
}

set<int> BCartesianProduct::getVariables() {
  set<int> result = left_->getVariables();
  result.merge(right_->getVariables());
  return result;
}

AbstractBType::shared_ptr BCartesianProduct::getLeft() { return left_; }

AbstractBType::shared_ptr BCartesianProduct::getRight() { return right_; }

// Implementation of the BPow class

BPow::BPow(AbstractBType::shared_ptr type) : type_(type) {}

string BPow::toSMT() { return "(POW " + type_->toSMT() + ")"; }

Term BPow::getTerm(SmtSolver solver, Sort sort) {
  Term internal_term = type_->getTerm(solver, sort);
  Term pow = solver->get_constructor(sort, "POW");
  return solver->make_term(Apply_Constructor, pow, internal_term);
}

bool BPow::contains(AbstractSolverElement::shared_ptr var) {
  return type_->contains(var);
}

string BPow::toString() { return "POW(" + type_->toString() + ")"; }

set<int> BPow::getVariables() { return type_->getVariables(); }

AbstractBType::shared_ptr BPow::getType() { return type_; }

// Implementation of the Variable class

int Variable::cpt_ = 0;

Variable::Variable(string id) : id_(id), numeric_id_(cpt_++) {}

string Variable::getSMTDeclaration() {
  return "(declare-fun " + id_ + " () Type)";
}

Term Variable::getTerm(SmtSolver solver, Sort sort) {
  return solver->get_symbol(id_);
}

string Variable::toSMT() { return id_; }

bool Variable::contains(AbstractSolverElement::shared_ptr var) {
  return var->toSMT() == toSMT();
}

string Variable::toString() { return id_; }

set<int> Variable::getVariables() { return {numeric_id_}; }

int Variable::getNumericId() { return numeric_id_; }
}  // namespace solver
