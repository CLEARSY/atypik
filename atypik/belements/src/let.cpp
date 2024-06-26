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
#include "let.h"

using std::string;
using std::vector;

namespace belem {
Let::Let(vector<Expression::shared_ptr> variables,
         vector<Expression::shared_ptr> values, Instruction::shared_ptr body)
    : variables_(variables), values_(values), body_(body) {}

string Let::format() {
  string result = "LET (";
  for (auto&& var : variables_) {
    result += var->format() + ",";
  }
  result.pop_back();
  result += ") BE ";
  for (auto&& val : values_) {
    result += val->format() + "||";
  }
  result.pop_back();
  result.pop_back();
  result += " IN ";
  result += body_->format();
  return result + " END";
}
}  // namespace belem
