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
#ifndef MODEL_H
#define MODEL_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "abstractsolverelement.h"
#include "assertion.h"
#include "btypes.h"

namespace smt
{
  class AbsSmtSolver;
  using SmtSolver = std::shared_ptr<AbsSmtSolver>;
  class AbsTerm;
  using Term = std::shared_ptr<AbsTerm>;
  class AbsSort;
  using Sort = std::shared_ptr<AbsSort>;
  class AbsDatatypeDecl;
  using DatatypeDecl = std::shared_ptr<AbsDatatypeDecl>;
  using UnorderedTermSet = std::unordered_set<Term>;
}

namespace solver
{

  /*!
   * \brief The Model class represents a Model which contains
   * assertions and variables
   */
  class Model : public AbstractSolverElement
  {
  public:
    /*!
     * \brief Construct an empty model
     */
    Model();
    /*!
     * \brief A shared pointer on a Model
     */
    typedef std::shared_ptr<Model> shared_ptr;
    std::string toSMT() override;
    bool contains(AbstractSolverElement::shared_ptr var) override;
    std::set<int> getVariables() override;
    /*!
     * \brief Add an assertion to the Model
     * \param assertion
     * The assertion to add
     */
    void add(Assertion::shared_ptr assertion);
    /*!
     * \brief Add a variable to the Model
     * \param variable
     * The variable to add
     */
    void add(Variable::shared_ptr variable);
    /*!
     * \brief Add an identifier to the model
     * \param id
     * The identifier to add
     */
    void add(BIdent::shared_ptr id);
    /*!
     * \brief Return a solution to the current model if it is sat.
     * \return A map of shape variable -> type where variable is a variable of
     * the model and type is the infered type. Raises a execption if the model
     * is unsat.
     */
    std::unordered_map<Variable::shared_ptr, std::string> solve();
    /*!
     * \brief Merge the variables and assertions of the current model with another one
     * \param model
     * The model which will be merged with the current one
     */
    void merge(Model::shared_ptr model);
    /**
     * @brief Replace all variables introduce by a let in a term by its value
     *
     * @param term
     * The term containing a let
     * @return The instanciated term
     */
    static std::string instanciateTerm(std::string &term);

  private:
    /*!
     * \brief The solver
     */
    smt::SmtSolver solver_;
    /*!
     * \brief The sort of the type type
     */
    smt::Sort type_sort_;
    /*!
     * \brief The declaration of the types type in the solver
     */
    smt::DatatypeDecl types_;
    /*!
     * \brief The terms of the model indexed by their name
     */
    std::unordered_map<std::string, smt::Term> terms_;
    /*!
     * \brief The types of data in the model
     */
    std::vector<BIdent::shared_ptr> datatypes_;
    /*!
     * \brief The assertions
     */
    std::unordered_set<Assertion::shared_ptr> assertions_;
    /*!
     * \brief The variables of the problem associated to their id
     */
    std::unordered_set<Variable::shared_ptr> variables_;
    /*!
     * \brief A boolean checking if the solver has been initialized
     */
    bool initialized_;
    /*!
     * \brief Compute a SMT declaration of the used data types
     * \return a SMT declaration of the used data types
     */
    std::string SMTTypeDeclaration();
    /*!
     * \brief Compute a SMT representation of the variables declaration
     * \return the SMT representation of the variables declaration
     */
    std::string SMTVariablesDeclaration();
    /*!
     * \brief Compute a SMT representation of the assertions of the model
     * \return the SMT representation of the assertions of the model
     */
    std::string SMTAssertions();
    /*!
     * \brief Compute a SMT representation of the commands needed to get
     * the variables values
     * \return a SMT representation of the commands needed to get
     * the variables values
     */
    std::string SMTGetValues();
    /*!
     * \brief Compute a SMT representation of the options used to solve the model
     * \return a SMT representation of the options used to solve the model
     */
    std::string SMTOptions();
    /*!
     * \brief Set the solver options
     */
    void setOptions();
    /*!
     * \brief Add the default datatypes to the solver
     */
    void addDataTypes();
    /*!
     * \brief Add the variables to the solver
     */
    void addVariables();
  };
} // namespace solver

#endif // MODEL_H
