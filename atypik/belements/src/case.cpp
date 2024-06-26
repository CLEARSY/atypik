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
#include "case.h"

using std::string;
using std::vector;

namespace belem {
Case::Case(Expression::shared_ptr value, vector<Expression::shared_ptr> choices,
           vector<Instruction::shared_ptr> consequences,
           Instruction::shared_ptr alternative)
    : value_(value),
      choices_(choices),
      consequences_(consequences),
      alternative_(alternative) {}

string Case::format() {
  string result = "CASE " + value_->format() + " OF " + " EITHER " +
                  choices_[0]->format() + " THEN " +
                  consequences_[0]->format() + " ";
  for (unsigned int i = 1; i < choices_.size(); i++) {
    result += "OR " + choices_[i]->format() + " THEN " +
              consequences_[i]->format() + " ";
  }
  if (alternative_ != nullptr) result += "ELSE " + alternative_->format() + " ";
  return result + "END";
}
}  // namespace belem
