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
#ifndef POGPARSER_H
#define POGPARSER_H

#include <memory>
#include <set>
#include <tinyxml2.h>
#include <unordered_map>

#include "context.h"
#include "bxmlposition.h"
#include "belemfactory.h"
#include "model.h"
#include "parser.h"
#include "solverfactory.h"

namespace pog
{

class Parser : public genericparser::Parser
{
public:
    /*!
     * \brief A unique_ptr on a Parser
     */
    typedef std::unique_ptr<Parser> unique_ptr;
    genericparser::Context::shared_ptr parse(tinyxml2::XMLDocument *pDocument) override;
    /*!
     * \brief Set the type of the bxml from which the pog file was generated, thus allowing
     * abstract sets to be of type POW(INTEGER)
     * \param type
     * The type of the initial bxml file
     */
    void setMachineType(genericparser::MachineType type);

private:
    /*!
     * \brief The models produced by the parser
     */
    std::vector<solver::Model::shared_ptr> models_;
    /*!
     * \brief A map containing the context of each definition
     */
    std::unordered_map<std::string, genericparser::Context::shared_ptr> def_to_context_;
    /*!
     * \brief A map containing the context of each local hypothesis that may be referenced
     */
    std::unordered_map<int, genericparser::Context::shared_ptr> num_to_local_hyp_;
    /*!
     * \brief Parse the define tags and create a local context
     * for each of it stored into the def_to_context map
     * \param pPo
     * A pointer on the Proof_Obligations tag
     */
    void parseDefines(tinyxml2::XMLElement *pPo);
    /*!
     * \brief Call parseProofObligation on each Proof_Obligation tag
     * \param pPo
     * A pointer on the Proof_Obligations tag
     */
    void parseProofObligations(tinyxml2::XMLElement *pPo);
    /*!
     * \brief Parse the Proof_Obligation type. The method creates a new context
     * for each proof obligation
     * \param pPos
     * A pointer on the Proof_Obligations tag
     * \param pPo
     * A pointer on the Proof_Obligation tag to parse
     * \param model
     * The model
     */
    void parseProofObligation(tinyxml2::XMLElement *pPos,
                              tinyxml2::XMLElement *pPo,
                              solver::Model::shared_ptr model);
    /*!
     * \brief Initialize one model per proof obligation to maximize multi threading
     * \param pPo
     * A pointer on the Proof_Obligations tag
     */
    void initModels(tinyxml2::XMLElement *pPo);
};
}

#endif // POGPARSER_H
