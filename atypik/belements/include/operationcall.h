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
#ifndef OPERATIONCALL_H
#define OPERATIONCALL_H

#include "expression.h"
#include "instruction.h"

namespace belem {
class OperationCall : public Instruction {
public:
    /*!
     * \brief Construct a B operation call
     * \param name
     * The name of the operation to call
     * \param inputs
     * The input arguments
     * \param outputs
     * The output arguments
     */
    OperationCall(std::string name,
                  std::vector<Expression::shared_ptr> inputs,
                  std::vector<Expression::shared_ptr> outputs);
    std::string format() override;

private:
    /*!
     * \brief The name of the operation to call
     */
    std::string name_;
    /*!
     * \brief The input arguments
     */
    std::vector<Expression::shared_ptr> inputs_;
    /*!
     * \brief The output arguments
     */
    std::vector<Expression::shared_ptr> outputs_;
};
}

#endif // OPERATIONCALL_H
