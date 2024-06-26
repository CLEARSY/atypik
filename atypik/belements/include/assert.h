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
#ifndef BASSERT_H
#define BASSERT_H

#include "instruction.h"
#include "predicate.h"

namespace belem {
class Assert : public Instruction{
public:
    /*!
     * \brief Construct an assertion from a condition and a body
     * \param condition
     * The condition
     * \param body
     * The body
     */
    Assert(Predicate::shared_ptr condition, Instruction::shared_ptr body);
    std::string format() override;

private:
    /*!
     * \brief The condition of the assertion
     */
    Predicate::shared_ptr condition_;
    /*!
     * \brief The body of the assertion
     */
    Instruction::shared_ptr body_;
};
}

#endif // BASSERT_H
