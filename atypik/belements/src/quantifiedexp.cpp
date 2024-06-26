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
#include "quantifiedexp.h"

using namespace belem;
using std::string;
using std::vector;

QuantifiedExp::QuantifiedExp(string type,
                             vector<Expression::shared_ptr> identifiers,
                             Predicate::shared_ptr predicate,
                             Expression::shared_ptr expression,
                             bxml::Position::shared_ptr position)
    : type_(type),
      identifiers_(identifiers),
      expression_(expression),
      predicate_(predicate),
      Expression({position}) {}

string QuantifiedExp::format() {
  string result = type_ + " (";
  for (auto&& id : identifiers_) result += id->format() + ", ";
  result.pop_back();
  result.pop_back();
  result += ").(" + predicate_->format() + " | ";
  result += expression_->format() + ")";
  return result;
}
