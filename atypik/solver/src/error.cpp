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
#include "error.h"

#include <regex>

#include "cvc5_factory.h"
#include "smt.h"

using namespace smt;

using std::pair;
using std::regex;
using std::regex_replace;
using std::string;
using std::unordered_map;
using std::vector;

namespace solver {

SolverError::SolverError(std::string message) : message_(message) {}

SolverError::SolverError(UnorderedTermSet unsolved_terms) {
  message_ =
      "Model is unsatisfiable. The following constraints are not compatible:\n";
  for (auto&& term : unsolved_terms) {
    int cpt = 0;
    for (auto it = term->begin(); it != term->end(); it++) {
      if (cpt == 0)
        message_ += "\t" + (*it)->to_string() + " = ";
      else if (cpt == 1)
        message_ += (*it)->to_string() + "\n";
      cpt++;
    }
  }
}

const char* SolverError::what() const noexcept { return message_.c_str(); }

void SolverError::replaceTerms(unordered_map<string, string> terms) {
  for (auto [old_term, new_term] : terms) {
    regex re(string(R"()") + old_term + "\\)" + string(R"()"));
    message_ = regex_replace(message_, re, new_term + ")");
    re = string(R"()") + old_term + " " + string(R"()");
    message_ = regex_replace(message_, re, new_term + " ");
    re = string(R"()") + old_term + "\n" + string(R"()");
    message_ = regex_replace(message_, re, new_term + "\n");
  }
}

}  // namespace solver
