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
#ifndef SELECT_H
#define SELECT_H

#include <vector>

#include "instruction.h"
#include "predicate.h"

namespace belem {
class Select : public Instruction {
public:
    /*!
     * \brief Construct a B select instruction
     * \param conditions
     * The conditions
     * \param consequences
     * The consequences associated to the conditions
     * \param alternative
     * The alternative
     */
    Select(std::vector<Predicate::shared_ptr> conditions,
           std::vector<Instruction::shared_ptr> consequences,
           Predicate::shared_ptr alternative);
    std::string format() override;

private:
    /*!
     * \brief The conditions
     */
    std::vector<Predicate::shared_ptr> conditions_;
    /*!
     * \brief The consequences
     */
    std::vector<Instruction::shared_ptr> consequences_;
    /*!
     * \brief The alternative
     */
    Predicate::shared_ptr alternative_;
};
}

#endif // SELECT_H
