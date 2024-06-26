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
#include "pogparser.h"

#include <iostream>

#include "vargen.h"

using namespace tinyxml2;

using belem::Expression;
using genericparser::Context;
using solver::Model;
using std::make_shared;
using std::pair;
using std::string;
using std::vector;

namespace pog {
// Implementation of the Parser class

Context::shared_ptr Parser::parse(XMLDocument *pDocument) {
  expressions_ = {};
  Context::shared_ptr context = make_shared<Context>();
  XMLElement *pPo = pDocument->FirstChildElement("Proof_Obligations");
  initModels(pPo);
  parseDefines(pPo);

  parseProofObligations(pPo);

  context->SetModels(models_);
  context->setExpressions(expressions_);

  return context;
}

void Parser::parseDefines(XMLElement *pPo) {
  Context::shared_ptr defines_context = make_shared<Context>();
  Model::shared_ptr tmp_model = initModel();
  for (XMLElement *pDefine = pPo->FirstChildElement("Define");
       pDefine != nullptr; pDefine = pDefine->NextSiblingElement("Define")) {
    Context::shared_ptr previous_context = defines_context->copy_shared_ptr();
    for (XMLElement *pTag = pDefine->FirstChildElement(); pTag != nullptr;
         pTag = pTag->NextSiblingElement()) {
      if (strcmp(pTag->Value(), "Set") == 0) {
        string tag_id;
        if (pTag->Attribute("tag")) tag_id = pTag->Attribute("tag");
        bxml::Position::shared_ptr pos =
            getPosition(pTag->FirstChildElement("Attr"), pTag);
        // The constraints can be added to only the global model
        parseSet(pTag, defines_context, models_[0], pos, type_);
      } else
        parsePredicate(pTag, defines_context, models_[0]);
    }
    // The identifiers of the previous contexts should not be known by the local
    // one
    Context::shared_ptr local_context = defines_context->copy_shared_ptr();
    // Then they are removed
    local_context->removeIdentifiers(previous_context);
    // Adding a local context for each definition
    def_to_context_[pDefine->Attribute("name")] = local_context;
  }

  if (enable_multi_thread_)
    for (unsigned int i = 1; i < models_.size(); i++)
      models_[i]->merge(models_[0]);
}

void Parser::parseProofObligations(XMLElement *pPos) {
  int i = 1;
  for (XMLElement *pPo = pPos->FirstChildElement("Proof_Obligation");
       pPo != nullptr; pPo = pPo->NextSiblingElement("Proof_Obligation")) {
    sets_ = {};
    relations_ = {};
    sequences_ = {};
    if (enable_multi_thread_)
      parseProofObligation(pPos, pPo, models_[i++]);
    else
      parseProofObligation(pPos, pPo, models_[0]);
  }
}

void Parser::parseProofObligation(XMLElement *pPos, XMLElement *pPo,
                                  Model::shared_ptr model) {
  // Create a specific context for each proof obligation
  Context::shared_ptr context = make_shared<Context>();
  // Filling the context with the referenced definitions
  for (XMLElement *pDefinition = pPo->FirstChildElement("Definition");
       pDefinition != nullptr;
       pDefinition = pDefinition->NextSiblingElement("Definition")) {
    context = context->merge(def_to_context_[pDefinition->Attribute("name")]);
  }
  // Parsing the predicate in the Hypothesis tags if any is present
  for (XMLElement *pHyp = pPo->FirstChildElement("Hypothesis"); pHyp != nullptr;
       pHyp = pHyp->NextSiblingElement("Hypothesis")) {
    parsePredicate(pHyp->FirstChildElement(), context, model);
  }
  // Parsing the predicate in the Local_Hyp tags if any is present
  for (XMLElement *pHyp = pPo->FirstChildElement("Local_Hyp"); pHyp != nullptr;
       pHyp = pHyp->NextSiblingElement("Local_Hyp")) {
    Context::shared_ptr local_context = context->copy_shared_ptr();
    int num = pHyp->Int64Attribute("num");
    parsePredicate(pHyp->FirstChildElement(), local_context, model);
    num_to_local_hyp_[num] = local_context;
  }
  // Parsing Simple_Goal tags if any is present
  for (XMLElement *pSimple_goal = pPo->FirstChildElement("Simple_Goal");
       pSimple_goal != nullptr;
       pSimple_goal = pSimple_goal->NextSiblingElement("Simple_Goal")) {
    Context::shared_ptr goal_context = context->copy_shared_ptr();
    // Adding the local hypothesis to the context if any is present
    for (XMLElement *pRef = pSimple_goal->FirstChildElement("Ref_Hyp");
         pRef != nullptr; pRef = pRef->NextSiblingElement("Ref_Hyp")) {
      goal_context =
          goal_context->merge(num_to_local_hyp_[pRef->Int64Attribute("num")]);
    }
    // Parsing Goal tags if any is present
    for (XMLElement *pGoal = pSimple_goal->FirstChildElement("Goal");
         pGoal != nullptr; pGoal = pGoal->NextSiblingElement("Goal")) {
      parsePredicate(pGoal->FirstChildElement(), goal_context, model);
    }
  }
}

void Parser::initModels(tinyxml2::XMLElement *pPos) {
  // Adding a global model containing all sets definitions in the define
  models_.push_back(initModel());
  if (enable_multi_thread_)
    // Adding a model by proof obligation
    for (XMLElement *pPo = pPos->FirstChildElement("Proof_Obligation");
         pPo != nullptr; pPo = pPo->NextSiblingElement("Proof_Obligation"))
      models_.push_back(initModel());
}

void Parser::setMachineType(genericparser::MachineType type) { type_ = type; }

}  // namespace pog
