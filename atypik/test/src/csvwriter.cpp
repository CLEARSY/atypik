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
#include "csvwriter.h"

#include <fstream>
#include <iostream>

using namespace belem;
using namespace solver;

using std::cout;
using std::endl;
using std::ofstream;
using std::ostream;
using std::shared_ptr;
using std::streambuf;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

namespace test {
void CSVWriter::writeTemplate(
    unordered_set<Expression::shared_ptr> expressions,
    unordered_map<Variable::shared_ptr, string> var_to_type, string output,
    string delimiter) {
  // Setting the buffer to write in the selected output
  streambuf* buffer;
  ofstream of;
  if (output != "") {
    of.open(output);
    buffer = of.rdbuf();
  } else {
    buffer = cout.rdbuf();
  }
  ostream out(buffer);

  out << "Expression" << delimiter << "Identifier" << delimiter << "Line"
      << delimiter << "Column" << delimiter << "Type" << endl;

  // Get one of the positions of the expression
  for (auto&& expression : expressions) {
    bxml::Position::shared_ptr first_pos;
    for (auto&& position : expression->getPositions()) {
      first_pos = position;
      break;
    }

    // External expressions shouldn't be typed
    if (first_pos == nullptr) continue;

    Variable::shared_ptr variable = expression->getAssociatedVariable();
    string expected_type =
        var_to_type.contains(variable) ? var_to_type[variable] : "";

    out << "\"" << expression->format() << "\"" << delimiter
        << variable->getNumericId() << delimiter << first_pos->getLine()
        << delimiter << first_pos->getColumn() << delimiter << expected_type
        << endl;
  }
}
}  // namespace test
