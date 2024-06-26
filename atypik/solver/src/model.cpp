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
#include "model.h"

#include "chrono"
#include "cvc5_factory.h"
#include "error.h"
#include "smt.h"

#include <regex>

using namespace smt;

using std::dynamic_pointer_cast;
using std::set;
using std::shared_ptr;
using std::string;
using std::unordered_map;

namespace solver
{

  Model::Model()
      : initialized_(false), solver_(Cvc5SolverFactory::create(false)) {}

  std::string Model::toSMT()
  {
    return SMTOptions() + "\n" + SMTTypeDeclaration() + "\n" +
           SMTVariablesDeclaration() + "\n" + SMTAssertions() + "\n" +
           SMTGetValues() + "\n";
  }

  void Model::add(Assertion::shared_ptr to_add) { assertions_.emplace(to_add); }

  void Model::add(Variable::shared_ptr to_add) { variables_.emplace(to_add); }

  void Model::add(BIdent::shared_ptr to_add) { datatypes_.emplace_back(to_add); }

  string Model::SMTTypeDeclaration()
  {
    string result = "; Types declaration\n";
    result += "(declare-datatypes ((Type 0))\n\t((";
    result += "(POW (type Type)) (PRODUCT (a Type) (b Type)) ";
    for (auto &&type : datatypes_)
    {
      result += "(" + type->toSMT() + ") ";
    }
    result.pop_back();
    return result + ")))\n";
  }

  string Model::SMTVariablesDeclaration()
  {
    string result = "; Variable declarations\n";
    for (auto &&variable : variables_)
    {
      result += variable->getSMTDeclaration() + "\n";
    }
    return result;
  }

  string Model::SMTAssertions()
  {
    string result = "; Assertions\n";
    for (auto &&assertion : assertions_)
    {
      result += assertion->toSMT() + "\n";
    }
    return result;
  }

  string Model::SMTGetValues()
  {
    string result = "; Retrieving results\n(check-sat)";
    if (variables_.size() == 0)
      return result;

    result += "\n(get-value (";
    for (auto &&variable : variables_)
    {
      result += variable->toSMT() + " ";
    }
    result.pop_back();
    return result + "))";
  }

  string Model::SMTOptions()
  {
    return "; Setting the solver options \n"
           "(set-option :produce-unsat-cores true)\n";
  }

  void Model::setOptions()
  {
    solver_ = Cvc5SolverFactory::create(false);
    solver_->set_opt("produce-models", "true");
    solver_->set_opt("produce-unsat-assumptions", "true");
    solver_->set_logic("QF_UFDT");
  }

  void Model::addDataTypes()
  {
    types_ = solver_->make_datatype_decl("types");
    DatatypeConstructorDecl pow = solver_->make_datatype_constructor_decl("POW");
    DatatypeConstructorDecl prod =
        solver_->make_datatype_constructor_decl("PRODUCT");
    solver_->add_selector_self(pow, "T");
    solver_->add_selector_self(prod, "A");
    solver_->add_selector_self(prod, "B");
    solver_->add_constructor(types_, pow);
    solver_->add_constructor(types_, prod);
    for (auto &&datatype : datatypes_)
    {
      string type_name = datatype->toSMT();
      DatatypeConstructorDecl type =
          solver_->make_datatype_constructor_decl(type_name);
      solver_->add_constructor(types_, type);
    }
    type_sort_ = solver_->make_sort(types_);
  }

  void Model::addVariables()
  {
    for (auto &&variable : variables_)
    {
      string id = variable->toSMT();
      Term new_term = solver_->make_symbol(id, type_sort_);
      terms_[id] = new_term;
    }
  }

  void Model::merge(Model::shared_ptr model)
  {
    datatypes_.insert(datatypes_.end(), model->datatypes_.begin(),
                      model->datatypes_.end());
    variables_.insert(model->variables_.begin(), model->variables_.end());
    assertions_.insert(model->assertions_.begin(), model->assertions_.end());
  }

  string Model::instanciateTerm(string &term)
  {
    int open_parenthesis = 0;
    int current_index = 0;
    string current_word, var, val;
    // A boolean telling if we are reading a variable introduced by a let
    bool in_var = false;
    bool in_val = false;
    // The parenthesis of the let is always removed
    int removed_parenthesis = 1;

    // We look for the value of the introduced variable
    for (const auto &c : term)
    {
      if (c == ' ')
      {
        if (current_word == "(let")
        {
          in_var = true;
          current_word = "";
        }

        else if (in_var)
        {
          // The opening parenthesis must be removed
          current_word = current_word.substr(open_parenthesis, current_word.size());
          removed_parenthesis += open_parenthesis;
          var = current_word;
          in_var = false;
          in_val = true;
          current_word = "";
        }

        else if (in_val and open_parenthesis == 1)
        {
          current_word = current_word.substr(0, current_word.size() - 2);
          val = current_word;
          break;
        }
      }

      current_word += c;
      ++current_index;

      if (c == '(')
        ++open_parenthesis;

      if (c == ')')
        --open_parenthesis;
    }

    // Triming val
    while (val[0] == ' ')
      val = val.substr(1, val.size());

    // Removing "let( " and ")"
    term = term.substr(current_index, term.size());
    term.pop_back();

    while (term[0] == ' ')
      term.erase(0, 1);

    return std::regex_replace(term, std::regex(var), val);
  }

  unordered_map<Variable::shared_ptr, std::string> Model::solve()
  {
    if (not initialized_)
    {
      setOptions();
      addDataTypes();
      addVariables();
      initialized_ = true;
    }

    unordered_map<Variable::shared_ptr, string> result;

    UnorderedTermSet assertions;
    for (auto &assertion : assertions_)
    {
      try
      {
        assertions.insert(
            assertion->getConstraint()->getTerm(solver_, type_sort_));
      }
      catch (IncorrectUsageException e)
      {
        // If some terms are not found, then they are ignored
        continue;
      }
    }

    Result model_result = solver_->check_sat_assuming_set(assertions);

    if (not model_result.is_sat())
    {
      UnorderedTermSet unsolved;
      solver_->get_unsat_assumptions(unsolved);
      throw SolverError(unsolved);
    }

    for (auto &var : variables_)
    {
      string name = var->toSMT();
      string value = solver_->get_value(terms_[name])->to_string();
      while (value.find("let") != string::npos)
        value = Model::instanciateTerm(value);
      result[var] = value;
    }

    return result;
  }

  bool Model::contains(AbstractSolverElement::shared_ptr var)
  {
    for (auto &&variable : variables_)
    {
      if (variable->contains(var))
        return true;
    }
    return false;
  }

  set<int> Model::getVariables()
  {
    set<int> result;
    for (auto &&var : variables_)
      result.emplace(var->getNumericId());
    return result;
  }

} // namespace solver
