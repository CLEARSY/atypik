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
#ifndef NARYSUB_H
#define NARYSUB_H

#include "instruction.h"

#include <vector>

namespace belem
{
class NarySub : public Instruction
{
public:
    /*!
     * \brief Construct a substitution of arity n
     * \param instructions
     * The instructions in the n-arity substitution
     * \param op
     * The operator
     */
    NarySub(std::vector<Instruction::shared_ptr> instructions, std::string op);
    std::string format() override;
private:
    /*!
     * \brief The instructions in the n-arity substitution
     */
    std::vector<Instruction::shared_ptr> instructions_;
    /*!
     * \brief The operator
     */
    std::string op_;
};
}

#endif // NARYSUB_H
