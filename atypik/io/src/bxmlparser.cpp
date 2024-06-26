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
#include "bxmlparser.h"

#include <filesystem>
#include <iostream>

#include "vargen.h"

using namespace tinyxml2;

using belem::Expression;
using genericparser::computeType;
using genericparser::Context;
using genericparser::MachineType;
using solver::Model;
using std::make_shared;
using std::pair;
using std::string;
using std::vector;

namespace bxml {
// Implementation of the IdentifierOutOfScope exception

MachineNotFound::MachineNotFound(string name, vector<string> folders)
    : message_("Machine " + name + " cannot be found") {
  if (folders.size() == 0)
    message_ += " (no folder was given to find it)";
  else {
    message_ += " in the following folder(s) :\n";
    for (auto &&folder : folders) message_ += "\t-" + folder + "\n";
    message_.pop_back();
  }
}

const char *MachineNotFound::what() const noexcept { return message_.c_str(); }

// Implementation of the Parser class

Context::shared_ptr Parser::parse(XMLDocument *pDocument) {
  expressions_ = {};
  Model::shared_ptr model = initModel();
  Context::shared_ptr context = make_shared<Context>();

  XMLElement *pMachine = pDocument->FirstChildElement("Machine");
  XMLElement *pParameters = pMachine->FirstChildElement("Parameters");
  XMLElement *pAbstraction = pMachine->FirstChildElement("Abstraction");
  XMLElement *pImports = pMachine->FirstChildElement("Imports");
  XMLElement *pIncludes = pMachine->FirstChildElement("Includes");
  XMLElement *pExtends = pMachine->FirstChildElement("Extends");
  XMLElement *pSees = pMachine->FirstChildElement("Sees");
  XMLElement *pAbsVars = pMachine->FirstChildElement("Abstract_Variables");
  XMLElement *pConcreteVars = pMachine->FirstChildElement("Concrete_Variables");
  XMLElement *pAbsCsts = pMachine->FirstChildElement("Abstract_Constants");
  XMLElement *pConcreteCsts = pMachine->FirstChildElement("Concrete_Constants");
  XMLElement *pInvariant = pMachine->FirstChildElement("Invariant");
  XMLElement *pProperties = pMachine->FirstChildElement("Properties");
  XMLElement *pAssertions = pMachine->FirstChildElement("Assertions");
  XMLElement *pConstraints = pMachine->FirstChildElement("Constraints");
  XMLElement *pInitialisation = pMachine->FirstChildElement("Initialisation");
  XMLElement *pSets = pMachine->FirstChildElement("Sets");
  XMLElement *pValues = pMachine->FirstChildElement("Values");
  XMLElement *pOperations = pMachine->FirstChildElement("Operations");
  XMLElement *pLocalOperations =
      pMachine->FirstChildElement("Local_Operations");

  string str_type = pMachine->Attribute("type");
  type_ = computeType(str_type);

  // Parsing external machines
  parseAbstraction(pAbstraction, context, model);
  parseMachinesWithoutParams(pImports, context, model);
  parseMachinesWithoutParams(pIncludes, context, model);
  parseMachinesWithoutParams(pExtends, context, model);
  parseMachinesWithoutParams(pSees, context, model);

  // Positions of external machines should be removed because they are out
  // of the analysed file
  for (auto &exp : expressions_) exp->clearPositions();

  parseParameters(pParameters, context, model, type_);
  parseSets(pSets, context, model, type_);
  parseValues(pValues, context, model);
  parseIdentifiers(pAbsVars, context, model);
  parseIdentifiers(pConcreteVars, context, model);
  parseIdentifiers(pAbsCsts, context, model);
  parseIdentifiers(pConcreteCsts, context, model);
  parseOperations(pLocalOperations, context, model);
  parseOperations(pOperations, context, model);
  parsePredicates(pInvariant, context, model);
  parsePredicates(pProperties, context, model);
  parsePredicates(pAssertions, context, model);
  parsePredicates(pConstraints, context, model);
  parseInitialisation(pInitialisation, context, model);

  context->SetModels({model});
  context->setExpressions(expressions_);
  context->setOperations(operations_);

  return context;
}

void Parser::addFolders(std::vector<std::string> folders) {
  folders_.insert(folders_.end(), folders.begin(), folders.end());
}

string Parser::findMachinePath(string name) {
  for (auto &&folder : folders_) {
    string path = folder + "/" + name + ".bxml";
    if (std::filesystem::exists(path)) return path;
  }
  throw MachineNotFound(name, folders_);
}

void Parser::parseMachinesWithoutParams(
    XMLElement *pMachines, genericparser::Context::shared_ptr context,
    solver::Model::shared_ptr model) {
  if (pMachines == nullptr) return;
  string old_prefix = prefix_;
  for (XMLElement *pRef = pMachines->FirstChildElement("Referenced_Machine");
       pRef != nullptr; pRef = pRef->NextSiblingElement("Referenced_Machine")) {
    string machine_name = pRef->FirstChildElement("Name")->GetText();
    string path = findMachinePath(machine_name);
    XMLElement *pInstance = pRef->FirstChildElement("Instance");
    if (pInstance != nullptr) {
      string instance = pInstance->GetText();
      prefix_ = instance + "." + prefix_;
    }
    string full_name = prefix_ + machine_name;
    // If the machine has already been parsed, there is no need to do it again
    if (parsed_machines_.contains(full_name)) continue;
    XMLDocument doc;
    doc.LoadFile(path.c_str());
    XMLDocument *pDoc = doc.ToDocument();
    XMLElement *pMachine = pDoc->FirstChildElement("Machine");
    parsed_machines_.insert(full_name);
    parseMachineWithoutParams(pMachine, context, model);
    prefix_ = old_prefix;
  }
}

void Parser::parseMachineWithoutParams(
    XMLElement *pMachine, genericparser::Context::shared_ptr context,
    solver::Model::shared_ptr model) {
  XMLElement *pAbsVars = pMachine->FirstChildElement("Abstract_Variables");
  XMLElement *pAbsCsts = pMachine->FirstChildElement("Abstract_Constants");
  XMLElement *pConcreteCsts = pMachine->FirstChildElement("Concrete_Constants");
  XMLElement *pConcreteVars = pMachine->FirstChildElement("Concrete_Variables");
  XMLElement *pInvariant = pMachine->FirstChildElement("Invariant");
  XMLElement *pProperties = pMachine->FirstChildElement("Properties");
  XMLElement *pAssertions = pMachine->FirstChildElement("Assertions");
  XMLElement *pConstraints = pMachine->FirstChildElement("Constraints");
  XMLElement *pSets = pMachine->FirstChildElement("Sets");
  XMLElement *pValues = pMachine->FirstChildElement("Values");
  XMLElement *pOperations = pMachine->FirstChildElement("Operations");
  XMLElement *pLocalOperations =
      pMachine->FirstChildElement("Local_Operations");
  XMLElement *pAbstraction = pMachine->FirstChildElement("Abstraction");
  XMLElement *pImports = pMachine->FirstChildElement("Imports");
  XMLElement *pIncludes = pMachine->FirstChildElement("Includes");
  XMLElement *pExtends = pMachine->FirstChildElement("Extends");
  XMLElement *pSees = pMachine->FirstChildElement("Sees");

  parseSets(pSets, context, model, type_);
  parseValues(pValues, context, model);
  parseOperations(pLocalOperations, context, model);
  parseOperations(pOperations, context, model);
  parseIdentifiers(pAbsVars, context, model);
  parseIdentifiers(pAbsCsts, context, model);
  parseIdentifiers(pConcreteCsts, context, model);
  parseIdentifiers(pConcreteVars, context, model);
  parsePredicates(pInvariant, context, model);
  parsePredicates(pProperties, context, model);
  parsePredicates(pAssertions, context, model);
  parsePredicates(pConstraints, context, model);

  // Parsing external machines
  parseAbstraction(pAbstraction, context, model);
  parseMachinesWithoutParams(pImports, context, model);
  parseMachinesWithoutParams(pIncludes, context, model);
  parseMachinesWithoutParams(pExtends, context, model);
  parseMachinesWithoutParams(pSees, context, model);
}

void Parser::parseAbstraction(XMLElement *pAbstraction,
                              genericparser::Context::shared_ptr context,
                              solver::Model::shared_ptr model) {
  if (pAbstraction == nullptr) return;
  string machine_name = pAbstraction->GetText();
  string full_name = prefix_ + machine_name;
  // If the machine has already been parsed, there is no need to do it again
  if (parsed_machines_.contains(full_name)) return;
  string path = findMachinePath(machine_name);
  parsed_machines_.insert(full_name);

  XMLDocument doc;
  doc.LoadFile(path.c_str());
  XMLDocument *pDoc = doc.ToDocument();
  XMLElement *pMachine = pDoc->FirstChildElement("Machine");
  XMLElement *pAbstractionOfAbstraction =
      pMachine->FirstChildElement("Abstraction");
  // Recursively parsing abstractions
  parseAbstraction(pAbstractionOfAbstraction, context, model);
  // Parsing the current abstraction
  parseMachineWithoutParams(pMachine, context, model);
}

}  // namespace bxml
