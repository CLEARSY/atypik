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
#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "bxmlparser.h"
#include "expression.h"
#include "set.h"
#include "solverfactory.h"

namespace test {
class CSVWriter {
public:
    /*!
     * \brief Write a template for tests. It creates a csv file with a column
     * representing the expressions. The second represents the line and the third
     * the column of the expression in the bxml file. The last column should be
     * filled by the user to specify the expected type of the expression
     * \param expressions
     * The expressions to put in the file
     * \param var_to_type
     * A map associating to a variable a type. It will be used to generate placeholders
     * in the type column
     * \param delimiter
     * \param output
     * The path of the file to create. If it is empty, the result will be
     * display on the standard output
     * The delimter between the columns
     */
    static void writeTemplate(std::unordered_set<belem::Expression::shared_ptr> expressions,
                              std::unordered_map<solver::Variable::shared_ptr, std::string> var_to_type = {},
                              std::string output="",
                              std::string delimiter=";");
};
}

#endif // CSVWRITER_H
