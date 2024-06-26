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
#ifndef OPERATION_H
#define OPERATION_H

#include <vector>

#include "belement.h"
#include "ident.h"
#include "instruction.h"
#include "predicate.h"

namespace belem
{
class Operation : public AbstractBElement{
public:
    /*!
     * \brief Construct an operation
     * \param name
     * The name of the operation
     * \param input_params
     * The input parameters of the operation
     * \param output_params
     * The output parameters of the operation
     * \param precondition
     * The precondition
     * \param body
     * The body of the operation
     * \param var
     * The variable associated to the type of the operation
     */
    Operation(std::string name,
              std::vector<Ident::shared_ptr> input_params,
              std::vector<Ident::shared_ptr> output_params,
              Predicate::shared_ptr precondition,
              Instruction::shared_ptr body,
              solver::Variable::shared_ptr var);
    /*!
     * \brief An accessor on the variable representing the type of
     * the operation
     * \return a shared pointer on the variable representing the type
     * of the operation
     */
    solver::Variable::shared_ptr getAssociatedVariable();
    std::string format() override;
    /*!
     * \brief A shared_ptr on an Operation
     */
    typedef std::shared_ptr<Operation> shared_ptr;
    /*!
     * \brief An accessor on the input arguments
     * \return the input arguments
     */
    std::vector<Ident::shared_ptr> getInputParams();
    /*!
     * \brief An accessor on the output arguments
     * \return the output arguments
     */
    std::vector<Ident::shared_ptr> getOutputParams();

private:
    /*!
     * \brief The associated type variable of the expression
     */
    solver::Variable::shared_ptr var_;
    /*!
     * \brief The name of the operation
     */
    std::string name_;
    /*!
     * \brief The input parameters of the operation
     */
    std::vector<Ident::shared_ptr> input_params_;
    /*!
     * \brief The output parameters of the operation
     */
    std::vector<Ident::shared_ptr> output_params_;
    /*!
     * \brief The precondition of the operation
     */
    Predicate::shared_ptr precondition_;
    /*!
     * \brief The body of the operation
     */
    Instruction::shared_ptr body_;
};
}

#endif // OPERATION_H
