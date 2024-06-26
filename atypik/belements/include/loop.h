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
#ifndef LOOP_H
#define LOOP_H

#include "expression.h"
#include "instruction.h"
#include "predicate.h"

namespace belem {
class Loop : public Instruction {
public:
    /*!
     * \brief Create a B while instruction
     * \param condition
     * The condition of the loop
     * \param body
     * The body of the loop
     * \param invariant
     * The invariant of the loop
     * \param variant
     * The variant of the loop
     */
    Loop(Predicate::shared_ptr condition,
         Instruction::shared_ptr body,
         Predicate::shared_ptr invariant,
         Expression::shared_ptr variant);
    std::string format() override;

private:
    /*!
     * \brief The condition of the loop
     */
    Predicate::shared_ptr condition_;
    /*!
     * \brief The body of the loop
     */
    Instruction::shared_ptr body_;
    /*!
     * \brief The invariant of the loop
     */
    Predicate::shared_ptr invariant_;
    /*!
     * \brief The variant of the loop
     */
    Expression::shared_ptr variant_;
};
}

#endif // LOOP_H
