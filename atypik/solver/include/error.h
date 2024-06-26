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
#ifndef ERROR_H
#define ERROR_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace smt {
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

namespace solver {
class SolverError : public std::exception
{
public:
    /*!
     * \brief Construct a SolverError
     * \param message
     * The message
     */
    SolverError(std::string message);
    /*!
     * \brief Construct a solver error for an unsatisfiable model
     * \param unsolved_terms
     * The terms which canot be solved
     */
    SolverError(smt::UnorderedTermSet unsolved_terms);
    /*!
     * \brief Replace all the terms in the error message by the one associated to it
     * \param a map associating a term to a new one
     */
    void replaceTerms(std::unordered_map<std::string, std::string> terms);
    const char * what() const noexcept override;


private:
    /*!
     * \brief The message to display
     */
    std::string message_;

};

}

#endif // ERROR_H
