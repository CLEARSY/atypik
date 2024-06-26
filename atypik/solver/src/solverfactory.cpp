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
#include "solverfactory.h"

using namespace solver;
using std::make_shared;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

// Implementation of the ConstructorError exception

ConstructorError::ConstructorError(string msg) : message_(msg) {}

const char* ConstructorError::what() const noexcept { return message_.c_str(); }

Assertion::shared_ptr Factory::makeAssertion(
    AbstractConstraint::shared_ptr constraint) {
  string str_constraint = constraint->toString();
  if (assertions_.contains(str_constraint)) {
    return assertions_[str_constraint];
  }
  Assertion::shared_ptr result = make_shared<Assertion>(constraint);
  assertions_[str_constraint] = result;
  return result;
}

Assertion::shared_ptr Factory::makeAssertEquals(
    AbstractTerm::shared_ptr left, AbstractTerm::shared_ptr right) {
  return makeAssertion(makeEquals(left, right));
}

BIdent::shared_ptr Factory::makeBIdent(string id) {
  if (identifers_.contains(id)) return identifers_[id];
  BIdent::shared_ptr result = make_shared<BIdent>(id);
  identifers_[id] = result;
  return result;
}

BIdent::shared_ptr Factory::makeBool() { return makeBIdent("BOOL"); }

BIdent::shared_ptr Factory::makeInteger() { return makeBIdent("INTEGER"); }

BIdent::shared_ptr Factory::makeReal() { return makeBIdent("REAL"); }

BIdent::shared_ptr Factory::makeString() { return makeBIdent("STRING"); }

BCartesianProduct::shared_ptr Factory::makeBCartesianProduct(
    AbstractBType::shared_ptr left, AbstractBType::shared_ptr right) {
  return make_shared<BCartesianProduct>(left, right);
}

BCartesianProduct::shared_ptr Factory::makeBCartesianProduct(
    vector<AbstractBType::shared_ptr> types) {
  if (types.size() < 2)
    throw ConstructorError(
        "A cartesian product should have at least two types");
  AbstractBType::shared_ptr result = types[0];
  for (unsigned int i = 1; i < types.size(); i++) {
    result = makeBCartesianProduct(result, types[i]);
  }
  return std::static_pointer_cast<BCartesianProduct>(result);
}

BPow::shared_ptr Factory::makeBPow(AbstractBType::shared_ptr type) {
  if (power_sets_.contains(type)) return power_sets_[type];
  BPow::shared_ptr result = make_shared<BPow>(type);
  power_sets_[type] = result;
  return result;
}

Equals::shared_ptr Factory::makeEquals(AbstractTerm::shared_ptr left,
                                       AbstractTerm::shared_ptr right) {
  return make_shared<Equals>(left, right);
}

Or::shared_ptr Factory::makeOr(AbstractTerm::shared_ptr left,
                               AbstractTerm::shared_ptr right) {
  return make_shared<Or>(left, right);
}

Not::shared_ptr Factory::makeNot(AbstractTerm::shared_ptr term) {
  return make_shared<Not>(term);
}

Model::shared_ptr Factory::makeModel() { return make_shared<Model>(); }

Variable::shared_ptr Factory::makeVariable(string id) {
  return make_shared<Variable>(id);
}
