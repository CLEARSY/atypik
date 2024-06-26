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
#include "operation.h"

using solver::Variable;
using std::string;
using std::vector;

namespace belem {
Operation::Operation(string name, vector<Ident::shared_ptr> input_params,
                     vector<Ident::shared_ptr> output_params,
                     Predicate::shared_ptr precondition,
                     Instruction::shared_ptr body, Variable::shared_ptr var)
    : name_(name),
      input_params_(input_params),
      output_params_(output_params),
      precondition_(precondition),
      body_(body),
      var_(var) {}

Variable::shared_ptr Operation::getAssociatedVariable() { return var_; }

string Operation::format() { return name_; }

vector<Ident::shared_ptr> Operation::getInputParams() { return input_params_; }

vector<Ident::shared_ptr> Operation::getOutputParams() {
  return output_params_;
}

}  // namespace belem
