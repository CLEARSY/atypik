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
#include "context.h"

#include "solverfactory.h"

using namespace tinyxml2;

using belem::Expression;
using belem::Operation;
using solver::Model;
using std::copy;
using std::inserter;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

namespace genericparser {
// Implementation of the IdentifierOutOfScope exception

IdentifierOutOfScope::IdentifierOutOfScope(std::string id)
    : message_("Identifier " + id + " is out of scope") {}

const char* IdentifierOutOfScope::what() const noexcept {
  return message_.c_str();
}

// Implementation of the Context class

Context::Context(bool add_set) {
  if (add_set) {
    pushSet("INTEGER");
    pushSet("INT");
    pushSet("NAT");
    pushSet("NAT1");
    pushSet("NATURAL");
    pushSet("NATURAL1");
    pushSet("BOOL");
    pushSet("FLOAT");
    pushSet("REAL");
    pushSet("STRING");
  }
}

void Context::push(belem::Expression::shared_ptr identifier) {
  string id = identifier->format();
  identifiers_[id].emplace_back(identifier);
}

belem::Expression::shared_ptr Context::get(string identifier) {
  if (identifiers_.contains(identifier)) {
    return identifiers_[identifier].back();
  }
  throw IdentifierOutOfScope(identifier);
}

bool Context::containsSet(string name) { return sets_.contains(name); }

void Context::pushSet(string name) { sets_.emplace(name); }

void Context::removeSet(string name) { sets_.erase(name); }

void Context::setExpressions(
    unordered_set<belem::Expression::shared_ptr> expressions) {
  expressions_ = move(expressions);
}

void Context::setOperations(
    unordered_map<string, belem::Operation::shared_ptr> operations) {
  for (auto&& operation : operations) operations_.emplace(operation);
}

void Context::SetModels(vector<Model::shared_ptr> models) { models_ = models; }

const unordered_set<Expression::shared_ptr>& Context::getExpressions() const {
  return expressions_;
}

unordered_set<Operation::shared_ptr> Context::getOperations() {
  unordered_set<Operation::shared_ptr> result;
  for (auto&& [_, op] : operations_) result.emplace(op);
  return result;
}

vector<Model::shared_ptr> Context::getModels() { return models_; }

Context::shared_ptr Context::copy_shared_ptr() {
  Context::shared_ptr result = make_shared<Context>();
  result->identifiers_ = identifiers_;
  result->sets_ = sets_;
  result->expressions_ = expressions_;
  result->operations_ = operations_;
  return result;
}

Context::shared_ptr Context::merge(Context::shared_ptr context) {
  Context::shared_ptr result = copy_shared_ptr();
  result->identifiers_.insert(context->identifiers_.begin(),
                              context->identifiers_.end());
  result->sets_.insert(context->sets_.begin(), context->sets_.end());
  unordered_map<string, Expression::shared_ptr> name_to_exp;
  unordered_set<Expression::shared_ptr> new_expressions;
  for (auto& expression : result->expressions_)
    name_to_exp[expression->format()] = expression;
  for (auto& expression : context->expressions_)
    name_to_exp[expression->format()] = expression;
  for (auto& [_, expression] : name_to_exp) new_expressions.insert(expression);
  result->expressions_ = move(new_expressions);
  result->operations_.insert(context->operations_.begin(),
                             context->operations_.end());
  return result;
}

void Context::removeIdentifiers(Context::shared_ptr context) {
  for (auto& [name, _] : context->identifiers_) identifiers_.erase(name);
}

}  // namespace genericparser
