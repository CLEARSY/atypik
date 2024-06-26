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
#ifndef CONTEXT_H
#define CONTEXT_H

#include <unordered_set>
#include <string>

#include "expression.h"
#include "model.h"
#include "operation.h"

namespace genericparser
{

class IdentifierOutOfScope : public std::exception
{
public:
    /*!
     * \brief Construct an IdentifierOutOfScope exception from an id
     * \param id
     * The id of the not found identifier
     */
    IdentifierOutOfScope(std::string id);
    const char * what() const noexcept override;

private:
    std::string message_;
};

class Context
{
public:
    /*!
     * \brief Initialize a context with the basic B types
     * \param add_set
     * A boolean telling if the default sets should be added in the context
     */
    Context(bool add_set = false);
    /*!
     * \brief Push an identifier into the context. If the identifier
     * is already present, its value is overriden
     * \param identifier
     * The identifier to push
     */
    void push(belem::Expression::shared_ptr identifier);
    /*!
     * \brief Fetch an identifier in the context
     * \param identifier
     * The identifier to fetch
     * \return the identifier if it is in the context. Raises an exception
     * otherwise
     */
    belem::Expression::shared_ptr get(std::string identifier);
    /*!
     * \brief Check if the set name is in the context
     * \param name
     * The name of the set
     * \return true if the set is in the context, false otherwise
     */
    bool containsSet(std::string name);
    /*!
     * \brief Add a set in the current context
     * \param name
     * The name of the set
     */
    void pushSet(std::string name);
    /*!
     * \brief Remove a set in the current context if it exists
     * \param name
     * The name of the set
     */
    void removeSet(std::string name);
    /*!
     * \brief Set the expressions_ attribute
     * \param expressions
     * The new value of expressions_
     */
    void setExpressions(std::unordered_set<belem::Expression::shared_ptr> expressions);
    /*!
     * \brief Set the operations_ attribute
     * \param operations
     * The new value of operations_
     */
    void setOperations(std::unordered_map<std::string, belem::Operation::shared_ptr> operations);
    /*!
     * \brief Set the models of the context
     * \param to_add
     * The models
     */
    void SetModels(std::vector<solver::Model::shared_ptr> model);
    /*!
     * \brief An accessor on the expressions in the context
     * \return the expressions in the context
     */
    const std::unordered_set<belem::Expression::shared_ptr>& getExpressions() const;
    /*!
     * \brief An accessor on the operations in the context
     * \return the operations in the context
     */
    std::unordered_set<belem::Operation::shared_ptr> getOperations();
    /*!
     * \brief An accessor on the models of the context
     * \return a shared pointer on the models in the context
     */
    std::vector<solver::Model::shared_ptr> getModels();
    /*!
     * \brief A shared_ptr on a context
     */
    typedef std::shared_ptr<Context> shared_ptr;
    /*!
     * \brief Copy the current context
     * \return a shared pointer on the new context
     */
    Context::shared_ptr copy_shared_ptr();
    /*!
     * \brief Merge the current context with another one
     * \param context
     * The other context
     * \return the merged context
     */
    Context::shared_ptr merge(Context::shared_ptr context);
    /*!
     * \brief Remove the identifiers contained in the given context from the
     * current one.
     * \param context
     * The given context
     */
    void removeIdentifiers(Context::shared_ptr context);

private:
    /*!
     * \brief A map of shape id -> {exp1, ..., expn}
     * where id is a string representing an identifier and
     * {exp1, ..., expn} the corresponding expressions
     */
    std::unordered_map<std::string, std::vector<belem::Expression::shared_ptr>> identifiers_;
    /*!
     * \brief Contains the sets in the context
     */
    std::unordered_set<std::string> sets_;
    /*!
     * \brief The parsed expressions
     */
    std::unordered_set<belem::Expression::shared_ptr> expressions_;
    /*!
     * \brief The parsed operations associated to their name
     */
    std::unordered_map<std::string, belem::Operation::shared_ptr> operations_;
    /*!
     * \brief The state of the models in the context
     */
    std::vector<solver::Model::shared_ptr> models_;
};
}

#endif // CONTEXT_H
