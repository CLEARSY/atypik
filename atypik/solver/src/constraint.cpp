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
#include "constraint.h"

#include "smt.h"

using namespace smt;
using std::set;
using std::string;

namespace solver {
// Implementation of the AbstractConstraint class

unsigned int AbstractConstraint::cpt_ = 0;

AbstractConstraint::AbstractConstraint() {
  id_ = "constraint__" + std::to_string(cpt_++);
}

string AbstractConstraint::getId() { return id_; }

// Implementation of the Equals class

Equals::Equals(AbstractTerm::shared_ptr left, AbstractTerm::shared_ptr right)
    : left_(left), right_(right) {}

string Equals::toSMT() {
  return "(! (= " + left_->toSMT() + " " + right_->toSMT() + ") :named " +
         getId() + ")";
}

bool Equals::contains(AbstractSolverElement::shared_ptr var) {
  return left_->contains(var) or right_->contains(var);
}

set<int> Equals::getVariables() {
  set<int> result = left_->getVariables();
  result.merge(right_->getVariables());
  return result;
}

Term Equals::getTerm(SmtSolver solver, Sort sort) {
  return solver->make_term(Equal, left_->getTerm(solver, sort),
                           right_->getTerm(solver, sort));
}

string Equals::toString() {
  return left_->toString() + " = " + right_->toString();
}

// Implementation of the Or class

Or::Or(AbstractTerm::shared_ptr left, AbstractTerm::shared_ptr right)
    : left_(left), right_(right) {}

string Or::toSMT() {
  return "(! (or " + left_->toSMT() + " " + right_->toSMT() + ") :named " +
         getId() + ")";
}

bool Or::contains(AbstractSolverElement::shared_ptr var) {
  return left_->contains(var) or right_->contains(var);
}

set<int> Or::getVariables() {
  set<int> result = left_->getVariables();
  result.merge(right_->getVariables());
  return result;
}

Term Or::getTerm(SmtSolver solver, Sort sort) {
  return solver->make_term(smt::Or, left_->getTerm(solver, sort),
                           right_->getTerm(solver, sort));
}

string Or::toString() {
  return left_->toString() + " or " + right_->toString();
}

// Implementation of the Not class

Not::Not(AbstractTerm::shared_ptr term) : term_(term) {}

string Not::toSMT() {
  return "(! (or " + term_->toSMT() + ") :named " + getId() + ")";
}

bool Not::contains(AbstractSolverElement::shared_ptr var) {
  return term_->contains(var);
}

set<int> Not::getVariables() { return term_->getVariables(); }

Term Not::getTerm(SmtSolver solver, Sort sort) {
  return solver->make_term(smt::Not, term_->getTerm(solver, sort));
}

string Not::toString() { return "not" + term_->toString(); }

}  // namespace solver
