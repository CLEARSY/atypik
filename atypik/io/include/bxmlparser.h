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
#ifndef BXMLPARSER_H
#define BXMLPARSER_H

#include <memory>
#include <set>
#include <tinyxml2.h>

#include "context.h"
#include "bxmlposition.h"
#include "belemfactory.h"
#include "model.h"
#include "parser.h"
#include "solverfactory.h"

namespace bxml
{

class MachineNotFound : public std::exception
{
public:
    /*!
     * \brief Construct a MachineNotFound exception from an name
     * \param name
     * The name of the not found machine
     * \param folders
     * The folders in which the machine was search
     */
    MachineNotFound(std::string id, std::vector<std::string> folders);
    const char * what() const noexcept override;

private:
    std::string message_;
};

class Parser : public genericparser::Parser
{
public:
    /*!
     * \brief A unique_ptr on a Parser
     */
    typedef std::unique_ptr<Parser> unique_ptr;
    genericparser::Context::shared_ptr parse(tinyxml2::XMLDocument * pDocument) override;
    /*!
     * \brief Add the folders in which to parse the referenced machines
     * \param folders
     * The folders
     */
    void addFolders(std::vector<std::string> folders);

private:
    /*!
     * \brief The folders containing the referenced machines
     */
    std::vector<std::string> folders_;
    /*!
     * \brief The machines that have already been parsed
     */
    std::unordered_set<std::string> parsed_machines_;
    /*!
     * \brief Find the path of a machine from its name
     * \param name
     * The name of the machine
     * \return the path of the machine
     */
    std::string findMachinePath(std::string name);
    /*!
     * \brief Parse external machines without its parameters.
     * This method is used to parse imported, included, extended and
     * seen machines.
     * \param pMachines
     * The tag to parse
     * \param context
     * The context
     * \param model
     * The model
     */
    void parseMachinesWithoutParams(tinyxml2::XMLElement *pMachines,
                                    genericparser::Context::shared_ptr context,
                                    solver::Model::shared_ptr model);
    /*!
     * \brief Parse an external machine without its parameters.
     * This method is used to parse imported, included, extended and
     * seen machines.
     * \param pMachine
     * The tag to parse
     * \param context
     * The context
     * \param model
     * The model
     */
    void parseMachineWithoutParams(tinyxml2::XMLElement *pMachine,
                                   genericparser::Context::shared_ptr context,
                                   solver::Model::shared_ptr model);
    /*!
     * \brief Parse all tags of a machine. This method is used to parse an astraction of a
     * machine.
     * \param pAbstraction
     * The tag to parse
     * \param context
     * The context
     * \param model
     * The model
     */
    void parseAbstraction(tinyxml2::XMLElement *pAbstraction,
                          genericparser::Context::shared_ptr context,
                          solver::Model::shared_ptr model);
};
}

#endif // BXMLPARSER_H
