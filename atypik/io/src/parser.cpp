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
#include "parser.h"

#include <algorithm>

using namespace bxml;
using namespace tinyxml2;
using belem::Any;
using belem::Assert;
using belem::Assignments;
using belem::BBool;
using belem::BecomesIn;
using belem::BecomesSuchThat;
using belem::BinaryExp;
using belem::BinaryPred;
using belem::BInteger;
using belem::Block;
using belem::BooleanExpression;
using belem::BReal;
using belem::BString;
using belem::Case;
using belem::Comparison;
using belem::Conditional;
using belem::Expression;
using belem::Ident;
using belem::Instruction;
using belem::Let;
using belem::Loop;
using belem::NaryExp;
using belem::NaryPred;
using belem::NarySub;
using belem::Operation;
using belem::OperationCall;
using belem::Predicate;
using belem::QuantifiedExp;
using belem::QuantifiedPred;
using belem::QuantifiedSet;
using belem::Select;
using belem::Set;
using belem::UnaryExp;
using belem::UnaryPred;
using belem::Valuation;
using belem::VarIn;
using genericparser::Context;
using genericparser::IdentifierOutOfScope;
using genericparser::UnknownXmlElement;
using solver::AbstractBType;
using solver::Assertion;
using solver::BCartesianProduct;
using solver::BIdent;
using solver::BPow;
using solver::Equals;
using solver::Model;
using solver::VarGenerator;
using solver::Variable;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::pair;
using std::set;
using std::string;
using std::vector;

using std::string;

namespace genericparser {

// Implementation of the errors

UnknownXmlElement::UnknownXmlElement(string msg) : message_(msg) {}

const char *UnknownXmlElement::what() const noexcept {
  return message_.c_str();
}

ParserError::ParserError(string msg) : message_(msg) {}

const char *ParserError::what() const noexcept { return message_.c_str(); }

void Parser::disableMultiThread() { enable_multi_thread_ = false; }

void Parser::addExpression(Model::shared_ptr model,
                           Expression::shared_ptr expression) {
  if (not expressions_.contains(expression)) {
    model->add(expression->getAssociatedVariable());
    expressions_.insert(expression);
  }
}

// Implementation of the parser

Model::shared_ptr Parser::initModel() {
  solver::Factory factory = solver::Factory();
  Model::shared_ptr result = factory.makeModel();
  // Generic type
  result->add(factory.makeBIdent("A"));
  result->add(factory.makeBIdent("FLOAT"));
  result->add(factory.makeInteger());
  result->add(factory.makeBool());
  result->add(factory.makeReal());
  result->add(factory.makeString());
  return result;
}

Position::shared_ptr Parser::getPosition(XMLElement *pAttr,
                                         XMLElement *pElement) {
  int l = -1, c = -1, s = -1;
  if (pAttr != nullptr) {
    XMLElement *pPos = pAttr->FirstChildElement("Pos");
    if (pPos != nullptr) {
      l = pPos->Int64Attribute("l");
      c = pPos->Int64Attribute("c");
      s = pPos->Int64Attribute("s");
    }
  }
  return make_shared<Position>(l, c, s, pElement);
}

Expression::shared_ptr Parser::parseExpression(XMLElement *pExpression,
                                               Context::shared_ptr context,
                                               Model::shared_ptr model,
                                               bool lookup_in_context) {
  string tag = pExpression->Value();
  string tag_id;
  if (pExpression->Attribute("tag")) tag_id = pExpression->Attribute("tag");
  Position::shared_ptr pos =
      getPosition(pExpression->FirstChildElement("Attr"), pExpression);

  if (tag == "Id")
    return parseId(pExpression, context, model, pos, lookup_in_context);
  if (tag == "Boolean_Literal")
    return parseDefaultType(pExpression, context, model, "BOOL", pos);
  if (tag == "Integer_Literal")
    return parseDefaultType(pExpression, context, model, "INTEGER", pos);
  if (tag == "Real_Literal")
    return parseDefaultType(pExpression, context, model, "REAL", pos);
  if (tag == "String_Literal")
    return parseDefaultType(pExpression, context, model, "STRING", pos);
  if (tag == "Binary_Exp")
    return parseBinaryExpression(pExpression, context, model, pos);
  if (tag == "Unary_Exp")
    return parseUnaryExpression(pExpression, context, model, pos);
  if (tag == "Boolean_Exp")
    return parseBooleanExpression(pExpression, context, model, pos);
  if (tag == "Nary_Exp")
    return parseNaryExpression(pExpression, context, model, pos);
  if (tag == "Set")
    return parseSet(pExpression, context, model, pos, Abstraction);
  if (tag == "EmptySeq") return parseEmptySeq(context, model, pos);
  if (tag == "EmptySet") return parseEmptySet(context, model, pos);
  if (tag == "Quantified_Set")
    return parseQuantifiedSet(pExpression, context, model, pos);
  if (tag == "Quantified_Exp")
    return parseQuantifiedExp(pExpression, context, model, pos);
  if (tag == "Valuation")
    return parseValuation(pExpression, context, model, pos);

  throw UnknownXmlElement(tag + " is an unknown tag for an expression");
}

Instruction::shared_ptr Parser::parseInstruction(XMLElement *pInstruction,
                                                 Context::shared_ptr context,
                                                 Model::shared_ptr model) {
  string tag = pInstruction->Value();
  Position::shared_ptr pos =
      getPosition(pInstruction->FirstChildElement("Attr"), pInstruction);
  if (tag == "Nary_Sub") return parseNarySub(pInstruction, context, model);
  if (tag == "Assignement_Sub")
    return parseAssignments(pInstruction, context, model);
  if (tag == "Bloc_Sub") return parseBlock(pInstruction, context, model);
  if (tag == "Skip") return belem::Factory::makeSkip();
  if (tag == "Assert_Sub") return parseAssertion(pInstruction, context, model);
  if (tag == "If_Sub") return parseIf(pInstruction, context, model);
  if (tag == "Select") return parseSelect(pInstruction, context, model);
  if (tag == "Case_Sub") return parseCase(pInstruction, context, model);
  if (tag == "ANY_Sub") return parseAny(pInstruction, context, model);
  if (tag == "LET_Sub") return parseLet(pInstruction, context, model);
  if (tag == "Becomes_In") return parseBecomesIn(pInstruction, context, model);
  if (tag == "Becomes_Such_That")
    return parseBecomesSuchThat(pInstruction, context, model);
  if (tag == "VAR_IN") return parseVarIn(pInstruction, context, model);
  if (tag == "Operation_Call")
    return parseOperationCall(pInstruction, context, model);
  if (tag == "While") return parseLoop(pInstruction, context, model);

  throw UnknownXmlElement(tag + " is an unknown tag for an instruction");
}

Predicate::shared_ptr Parser::parsePredicate(XMLElement *pPredicate,
                                             Context::shared_ptr context,
                                             Model::shared_ptr model) {
  string tag = pPredicate->Value();
  if (tag == "Unary_Pred") return parseUnaryPred(pPredicate, context, model);
  if (tag == "Nary_Pred") return parseNaryPred(pPredicate, context, model);
  if (tag == "Exp_Comparison")
    return parseComparison(pPredicate, context, model);
  if (tag == "Quantified_Pred")
    return parseQuantifiedPred(pPredicate, context, model);
  if (tag == "Binary_Pred") return parseBinaryPred(pPredicate, context, model);

  throw UnknownXmlElement(tag + " is an unknown tag for a predicate");
}

void Parser::parseIdentifiers(tinyxml2::XMLElement *pIdentifiers,
                              Context::shared_ptr context,
                              solver::Model::shared_ptr model) {
  if (pIdentifiers != nullptr) {
    for (XMLElement *pIdentifier = pIdentifiers->FirstChildElement("Id");
         pIdentifier != nullptr;
         pIdentifier = pIdentifier->NextSiblingElement("Id")) {
      parseExpression(pIdentifier, context, model);
    }
  }
}

void Parser::parsePredicates(XMLElement *pInvariant,
                             Context::shared_ptr context,
                             Model::shared_ptr model) {
  if (pInvariant != nullptr) {
    XMLElement *pPred =
        pInvariant->FirstChildElement("Attr")->NextSiblingElement();
    parsePredicate(pPred, context, model);
  }
}

void Parser::parseParameters(XMLElement *pParameters,
                             Context::shared_ptr context,
                             Model::shared_ptr model, MachineType type) {
  if (pParameters != nullptr) {
    for (XMLElement *pParam = pParameters->FirstChildElement("Id");
         pParam != nullptr; pParam = pParam->NextSiblingElement("Id")) {
      string name = pParam->Attribute("value");
      Position::shared_ptr pos =
          getPosition(pParam->FirstChildElement("Attr"), pParam);
      Ident::shared_ptr id = belem::Factory::makeIdent(name, pos);
      // If name is upper, it is a set
      if (std::all_of(name.begin(), name.end(), [](unsigned char c) {
            return std::isupper(c) or c == '_';
          })) {
        Set::shared_ptr set = belem::Factory::makeSet(id, {}, pos);
        if (type == Implementation)
          // The type of a set SET is POW(INTEGER) in an implementation
          model->add(s_factory_.makeAssertEquals(
              set->getAssociatedVariable(),
              s_factory_.makeBPow(s_factory_.makeInteger())));
        else {
          // Adding the new set type to the model
          model->add(s_factory_.makeBIdent(id->format()));
          global_context_->pushSet(id->format());

          // The type of a set SET is POW(SET) in an abstraction
          model->add(s_factory_.makeAssertEquals(
              set->getAssociatedVariable(),
              s_factory_.makeBPow(s_factory_.makeBIdent(id->format()))));
        }
        addExpression(model, set);
      } else
        parseId(pParam, context, model, pos);
    }
  }
}

void Parser::parseInitialisation(XMLElement *pInitialisation,
                                 Context::shared_ptr context,
                                 Model::shared_ptr model) {
  if (pInitialisation != nullptr) {
    XMLElement *pPosition = pInitialisation->FirstChildElement("Attr");
    if (pPosition != nullptr) {
      XMLElement *pInstr = pPosition->NextSiblingElement();
      parseInstruction(pInstr, context, model);
    }
  }
}

void Parser::parseSets(XMLElement *pSets, Context::shared_ptr context,
                       Model::shared_ptr model, MachineType type) {
  if (pSets != nullptr) {
    XMLElement *pPosition = pSets->FirstChildElement("Attr");
    if (pPosition != nullptr) {
      for (XMLElement *pSet = pPosition->NextSiblingElement(); pSet != nullptr;
           pSet = pSet->NextSiblingElement()) {
        Position::shared_ptr pos =
            getPosition(pSet->FirstChildElement("Attr"), pSet);
        parseSet(pSet, context, model, pos, type)->format();
      }
    }
  }
}

void Parser::parseValues(XMLElement *pValues,
                         genericparser::Context::shared_ptr context,
                         solver::Model::shared_ptr model) {
  if (pValues != nullptr) {
    for (XMLElement *pValue = pValues->FirstChildElement("Valuation");
         pValue != nullptr; pValue = pValue->NextSiblingElement("Valuation"))
      parseExpression(pValue, context, model);
  }
}

vector<Expression::shared_ptr> Parser::parseExpressions(
    XMLElement *pExpressions, Context::shared_ptr context,
    Model::shared_ptr model) {
  vector<Expression::shared_ptr> result;
  if (pExpressions != nullptr) {
    for (XMLElement *pExpression = pExpressions->FirstChildElement();
         pExpression != nullptr;
         pExpression = pExpression->NextSiblingElement())
      result.emplace_back(parseExpression(pExpression, context, model));
  }
  return result;
}

Ident::shared_ptr Parser::parseId(XMLElement *pId, Context::shared_ptr context,
                                  Model::shared_ptr model,
                                  Position::shared_ptr pos,
                                  bool lookup_in_context) {
  string name = pId->Attribute("value");
  const char *suffix = pId->Attribute("suffix");
  if (suffix != nullptr) name += suffix;
  Ident::shared_ptr identifier;
  try {
    if (not lookup_in_context) throw IdentifierOutOfScope("");
    identifier = context->get(prefix_ + name);
    identifier->addPosition(pos);
  } catch (IdentifierOutOfScope) {
    // If the identifier is a set
    if (global_context_->containsSet(name)) {
      try {
        identifier = global_context_->get(name);
        identifier->addPosition(pos);
      } catch (IdentifierOutOfScope) {
        identifier = belem::Factory::makeIdent(name, pos);
        global_context_->push(identifier);
      }
      // INT and NAT are a subsets of INTEGER
      if (name == "INT" or name == "NATURAL" or name == "NATURAL1" or
          name == "NAT" or name == "NAT1")
        model->add(s_factory_.makeAssertEquals(
            identifier->getAssociatedVariable(),
            s_factory_.makeBPow(s_factory_.makeInteger())));
      else
        model->add(s_factory_.makeAssertEquals(
            identifier->getAssociatedVariable(),
            s_factory_.makeBPow(s_factory_.makeBIdent(name))));
    } else {
      identifier = belem::Factory::makeIdent(prefix_ + name, pos);
      context->push(identifier);
    }
    addExpression(model, identifier);
  }
  return identifier;
}

Expression::shared_ptr Parser::parseDefaultType(XMLElement *pTag,
                                                Context::shared_ptr context,
                                                Model::shared_ptr model,
                                                string type_name,
                                                Position::shared_ptr pos) {
  const char *value = pTag->Attribute("value");
  Expression::shared_ptr result;
  if (type_name == "BOOL") result = belem::Factory::makeBool(value, pos);
  if (type_name == "INTEGER") result = belem::Factory::makeInt(value, pos);
  if (type_name == "REAL") result = belem::Factory::makeReal(value, pos);
  if (type_name == "STRING") result = belem::Factory::makeString(value, pos);

  addExpression(model, result);
  model->add(s_factory_.makeAssertEquals(result->getAssociatedVariable(),
                                         s_factory_.makeBIdent(type_name)));
  return result;
}

Set::shared_ptr Parser::parseSet(XMLElement *pSet, Context::shared_ptr context,
                                 Model::shared_ptr model,
                                 Position::shared_ptr pos, MachineType type) {
  XMLElement *pId = pSet->FirstChildElement("Id");
  XMLElement *pValues = pSet->FirstChildElement("Enumerated_Values");
  Ident::shared_ptr id =
      static_pointer_cast<Ident>(parseExpression(pId, context, model));
  vector<Expression::shared_ptr> values =
      parseExpressions(pValues, context, model);
  Set::shared_ptr set = belem::Factory::makeSet(id, values, pos);

  // In the case of an implementation, non enumerated sets should be valuated as
  // INTEGER sets
  if (values.size() == 0 and type == Implementation) {
    // The type of a set SET is POW(INTEGER) in an implementation
    model->add(s_factory_.makeAssertEquals(
        set->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeInteger())));
  } else {
    // Adding the new set type to the model
    model->add(s_factory_.makeBIdent(id->format()));
    global_context_->pushSet(id->format());

    // The type of a set SET is POW(SET) in an abstraction
    model->add(s_factory_.makeAssertEquals(
        set->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBIdent(id->format()))));

    // for SET = {Elt1, ..., Eltn}, SET and {Elt1, ..., Eltn} must have the same
    // type
    model->add(s_factory_.makeAssertEquals(id->getAssociatedVariable(),
                                           set->getAssociatedVariable()));

    // Each value in a set of type POW(T) must be of type T
    for (auto &&val : values) {
      model->add(s_factory_.makeAssertEquals(
          id->getAssociatedVariable(),
          s_factory_.makeBPow(val->getAssociatedVariable())));
    }
  }
  addExpression(model, set);
  return set;
}

NarySub::shared_ptr Parser::parseNarySub(XMLElement *pSub,
                                         Context::shared_ptr context,
                                         Model::shared_ptr model) {
  const char *op = pSub->Attribute("op");
  XMLElement *pStart;
  XMLElement *pPos = pSub->FirstChildElement("Attr");
  if (pPos == nullptr)
    pStart = pSub->FirstChildElement();
  else
    pStart = pPos->NextSiblingElement();
  vector<Instruction::shared_ptr> instructions;
  for (XMLElement *pContent = pStart; pContent != nullptr;
       pContent = pContent->NextSiblingElement())
    instructions.emplace_back(parseInstruction(pContent, context, model));
  return belem::Factory::makeNarySub(instructions, op);
}

Assignments::shared_ptr Parser::parseAssignments(XMLElement *pAssignments,
                                                 Context::shared_ptr context,
                                                 Model::shared_ptr model) {
  XMLElement *pVariables = pAssignments->FirstChildElement("Variables");
  XMLElement *pValues = pAssignments->FirstChildElement("Values");
  vector<Expression::shared_ptr> vars =
      parseExpressions(pVariables, context, model);
  vector<Expression::shared_ptr> vals =
      parseExpressions(pValues, context, model);
  for (unsigned int i = 0; i < vars.size(); i++) {
    model->add(s_factory_.makeAssertEquals(vars[i]->getAssociatedVariable(),
                                           vals[i]->getAssociatedVariable()));
  }
  return belem::Factory::makeAssignments(vars, vals);
}

Block::shared_ptr Parser::parseBlock(XMLElement *pBlock,
                                     Context::shared_ptr context,
                                     Model::shared_ptr model) {
  XMLElement *pPos = pBlock->FirstChildElement("Attr");
  XMLElement *pBody;
  if (pPos == nullptr)
    pBody = pBlock->FirstChildElement();
  else
    pBody = pPos->NextSiblingElement();
  Instruction::shared_ptr body = parseInstruction(pBody, context, model);
  return belem::Factory::makeBlock(body);
}

UnaryPred::shared_ptr Parser::parseUnaryPred(XMLElement *pPred,
                                             Context::shared_ptr context,
                                             Model::shared_ptr model) {
  string op = pPred->Attribute("op");
  XMLElement *pPos = pPred->FirstChildElement("Attr");
  XMLElement *pArg = pPred->FirstChildElement();
  if (pPos != nullptr) pArg = pArg->NextSiblingElement();
  Predicate::shared_ptr arg = parsePredicate(pArg, context, model);
  return belem::Factory::makeUnaryPred(op, arg);
}

NaryPred::shared_ptr Parser::parseNaryPred(XMLElement *pPred,
                                           Context::shared_ptr context,
                                           Model::shared_ptr model) {
  vector<Predicate::shared_ptr> clauses;
  string op = pPred->Attribute("op");
  for (XMLElement *pClause = pPred->FirstChildElement(); pClause != nullptr;
       pClause = pClause->NextSiblingElement())
    clauses.emplace_back(parsePredicate(pClause, context, model));
  return belem::Factory::makeNaryPred(clauses, op);
}

Comparison::shared_ptr Parser::parseComparison(XMLElement *pPred,
                                               Context::shared_ptr context,
                                               Model::shared_ptr model) {
  XMLElement *pPos = pPred->FirstChildElement("Attr");
  XMLElement *pLeft;
  if (pPos != nullptr)
    pLeft = pPos->NextSiblingElement();
  else
    pLeft = pPred->FirstChildElement();
  XMLElement *pRight = pLeft->NextSiblingElement();
  string op = pPred->Attribute("op");
  Expression::shared_ptr left = parseExpression(pLeft, context, model);
  Expression::shared_ptr right = parseExpression(pRight, context, model);
  if (op == ":" or op == "/:")
    // for expr1 : expr2 of types T1 and T2, T2 = POW(T1)
    model->add(s_factory_.makeAssertEquals(
        right->getAssociatedVariable(),
        s_factory_.makeBPow(left->getAssociatedVariable())));
  else if (op == "=" or op == "/=")
    // Both operands must have the same type
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(),
                                           left->getAssociatedVariable()));
  else if (op == "<=i" or op == ">=i" or op == "<i" or op == ">i") {
    // Both operand type is integer
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
  } else if (op == "<=r" or op == ">=r" or op == "<r" or op == ">r") {
    // Both operand type is real
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(),
                                           s_factory_.makeReal()));
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           s_factory_.makeReal()));
  } else if (op == "<:") {
    // Both operands must be sets
    Variable::shared_ptr t1 = addSetTypeConstraint(model, left);
    Variable::shared_ptr t2 = addSetTypeConstraint(model, right);
    // Left and right operands must have the same type
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           right->getAssociatedVariable()));
  } else
    throw UnknownXmlElement("<Exp_Comparison op=\"" + op + "\">");
  return belem::Factory::makeComparison(left, op, right);
}

Assert::shared_ptr Parser::parseAssertion(XMLElement *pAssertion,
                                          Context::shared_ptr context,
                                          Model::shared_ptr model) {
  XMLElement *pGuard = pAssertion->FirstChildElement("Guard");
  XMLElement *pBody = pAssertion->FirstChildElement("Body");
  Predicate::shared_ptr condition =
      parsePredicate(pGuard->FirstChildElement(), context, model);
  Instruction::shared_ptr body =
      parseInstruction(pBody->FirstChildElement(), context, model);
  return belem::Factory::makeAssert(condition, body);
}

Conditional::shared_ptr Parser::parseIf(XMLElement *pIf,
                                        Context::shared_ptr context,
                                        Model::shared_ptr model) {
  XMLElement *pCondition = pIf->FirstChildElement("Condition");
  XMLElement *pThen = pIf->FirstChildElement("Then");
  XMLElement *pElse = pIf->FirstChildElement("Else");
  Predicate::shared_ptr condition =
      parsePredicate(pCondition->FirstChildElement(), context, model);
  Instruction::shared_ptr consequent, alternative;
  consequent = parseInstruction(pThen->FirstChildElement(), context, model);
  if (pElse != nullptr)
    alternative = parseInstruction(pElse->FirstChildElement(), context, model);
  return belem::Factory::makeIf(condition, consequent, alternative);
}

Select::shared_ptr Parser::parseSelect(XMLElement *pSelect,
                                       Context::shared_ptr context,
                                       Model::shared_ptr model) {
  XMLElement *pClauses = pSelect->FirstChildElement("When_Clauses");
  XMLElement *pElse = pSelect->FirstChildElement("Else");
  vector<Predicate::shared_ptr> conditions;
  vector<Instruction::shared_ptr> consequences;
  Instruction::shared_ptr alternative;
  for (XMLElement *pWhen = pClauses->FirstChildElement("When");
       pWhen != nullptr; pWhen = pWhen->NextSiblingElement("When")) {
    conditions.emplace_back(parsePredicate(
        pWhen->FirstChildElement("Condition")->FirstChildElement(), context,
        model));
    consequences.emplace_back(parseInstruction(
        pWhen->FirstChildElement("Then")->FirstChildElement(), context, model));
  }
  if (pElse != nullptr)
    alternative = parseInstruction(pElse->FirstChildElement(), context, model);
  return belem::Factory::makeSelect(conditions, consequences, alternative);
}

Case::shared_ptr Parser::parseCase(XMLElement *pCase,
                                   Context::shared_ptr context,
                                   Model::shared_ptr model) {
  XMLElement *pValue = pCase->FirstChildElement("Value");
  XMLElement *pChoices = pCase->FirstChildElement("Choices");
  XMLElement *pElse = pCase->FirstChildElement("Else");
  Expression::shared_ptr value =
      parseExpression(pValue->FirstChildElement(), context, model);
  vector<Expression::shared_ptr> choices;
  vector<Instruction::shared_ptr> consequences;
  Instruction::shared_ptr alternative;
  for (XMLElement *pChoice = pChoices->FirstChildElement("Choice");
       pChoice != nullptr; pChoice = pChoice->NextSiblingElement("Choice")) {
    choices.emplace_back(parseExpression(
        pChoice->FirstChildElement("Value")->FirstChildElement(), context,
        model));
    consequences.emplace_back(parseInstruction(
        pChoice->FirstChildElement("Then")->FirstChildElement(), context,
        model));
  }
  if (pElse != nullptr)
    alternative = parseInstruction(pElse->FirstChildElement(), context, model);
  Case::shared_ptr bcase =
      belem::Factory::makeCase(value, choices, consequences, alternative);
  // value should not be of type POW(t)
  Variable::shared_ptr t = VarGenerator::getNewVariable();
  model->add(t);
  model->add(s_factory_.makeAssertion(s_factory_.makeNot(s_factory_.makeEquals(
      s_factory_.makeBPow(t), value->getAssociatedVariable()))));
  // value should not be of type t1 x t2
  Variable::shared_ptr t1 = VarGenerator::getNewVariable();
  Variable::shared_ptr t2 = VarGenerator::getNewVariable();
  model->add(t1);
  model->add(t2);
  model->add(s_factory_.makeAssertion(s_factory_.makeNot(
      s_factory_.makeEquals(s_factory_.makeBCartesianProduct(t1, t2),
                            value->getAssociatedVariable()))));
  // value should not be a STRING
  model->add(s_factory_.makeAssertion(s_factory_.makeNot(s_factory_.makeEquals(
      s_factory_.makeString(), value->getAssociatedVariable()))));
  // Each constant in the branches should have the same type as value
  for (auto &&cst : choices) {
    model->add(s_factory_.makeAssertEquals(cst->getAssociatedVariable(),
                                           value->getAssociatedVariable()));
  }
  return bcase;
}

Any::shared_ptr Parser::parseAny(XMLElement *pAny, Context::shared_ptr context,
                                 Model::shared_ptr model) {
  Context::shared_ptr local_context = context->copy_shared_ptr();
  XMLElement *pVariables = pAny->FirstChildElement("Variables");
  XMLElement *pPred = pAny->FirstChildElement("Pred");
  XMLElement *pThen = pAny->FirstChildElement("Then");
  vector<Expression::shared_ptr> variables;
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id"))
    variables.emplace_back(
        parseExpression(pVariable, local_context, model, false));
  Predicate::shared_ptr predicate =
      parsePredicate(pPred->FirstChildElement(), local_context, model);
  Instruction::shared_ptr body =
      parseInstruction(pThen->FirstChildElement(), local_context, model);
  return belem::Factory::makeAny(variables, predicate, body);
}

Let::shared_ptr Parser::parseLet(XMLElement *pLet, Context::shared_ptr context,
                                 Model::shared_ptr model) {
  Context::shared_ptr local_context = context->copy_shared_ptr();
  XMLElement *pVariables = pLet->FirstChildElement("Variables");
  XMLElement *pValues = pLet->FirstChildElement("Values");
  XMLElement *pThen = pLet->FirstChildElement("Then");
  vector<Expression::shared_ptr> variables, values;
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id"))
    variables.emplace_back(
        parseExpression(pVariable, local_context, model, false));
  for (XMLElement *pValue = pValues->FirstChildElement("Valuation");
       pValue != nullptr; pValue = pValue->NextSiblingElement("Valuation")) {
    values.emplace_back(parseExpression(pValue, local_context, model));
  }

  Instruction::shared_ptr body =
      parseInstruction(pThen->FirstChildElement(), local_context, model);
  return belem::Factory::makeLet(variables, values, body);
}

BecomesIn::shared_ptr Parser::parseBecomesIn(XMLElement *pBecomes,
                                             Context::shared_ptr context,
                                             Model::shared_ptr model) {
  XMLElement *pVariables = pBecomes->FirstChildElement("Variables");
  XMLElement *pValue = pBecomes->FirstChildElement("Value");
  vector<Expression::shared_ptr> variables;
  vector<AbstractBType::shared_ptr> variables_types;
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id")) {
    Expression::shared_ptr identifier =
        parseExpression(pVariable, context, model);
    variables.emplace_back(identifier);
    variables_types.emplace_back(identifier->getAssociatedVariable());
  }
  Expression::shared_ptr set =
      parseExpression(pValue->FirstChildElement(), context, model);
  AbstractBType::shared_ptr t;
  if (variables_types.size() == 1)
    t = variables_types[0];
  else
    t = s_factory_.makeBCartesianProduct(variables_types);
  // The type of the set should be POW(t) assuming the vector is of type t
  model->add(s_factory_.makeAssertEquals(set->getAssociatedVariable(),
                                         s_factory_.makeBPow(t)));
  return belem::Factory::makeBecomesIn(variables, set);
}

BecomesSuchThat::shared_ptr Parser::parseBecomesSuchThat(
    XMLElement *pBecomes, Context::shared_ptr context,
    Model::shared_ptr model) {
  XMLElement *pVariables = pBecomes->FirstChildElement("Variables");
  XMLElement *pPred = pBecomes->FirstChildElement("Pred");
  vector<Expression::shared_ptr> variables;
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id"))
    variables.emplace_back(parseExpression(pVariable, context, model));
  Predicate::shared_ptr pred =
      parsePredicate(pPred->FirstChildElement(), context, model);
  return belem::Factory::makeBecomesSuchThat(variables, pred);
}

VarIn::shared_ptr Parser::parseVarIn(XMLElement *pVarIn,
                                     Context::shared_ptr context,
                                     Model::shared_ptr model) {
  Context::shared_ptr local_context = context->copy_shared_ptr();
  XMLElement *pVariables = pVarIn->FirstChildElement("Variables");
  XMLElement *pBody = pVarIn->FirstChildElement("Body");
  vector<Expression::shared_ptr> variables;
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id"))
    variables.emplace_back(
        parseExpression(pVariable, local_context, model, false));
  Instruction::shared_ptr body =
      parseInstruction(pBody->FirstChildElement(), local_context, model);
  return belem::Factory::makeVarIn(variables, body);
}

OperationCall::shared_ptr Parser::parseOperationCall(
    XMLElement *pCall, Context::shared_ptr context, Model::shared_ptr model) {
  XMLElement *pName = pCall->FirstChildElement("Name");
  XMLElement *pInputs = pCall->FirstChildElement("Input_Parameters");
  XMLElement *pOutputs = pCall->FirstChildElement("Output_Parameters");
  Expression::shared_ptr name_id =
      parseExpression(pName->FirstChildElement("Id"), context, model);
  string name = name_id->format();
  vector<Expression::shared_ptr> inputs, outputs;
  if (pInputs)
    for (XMLElement *pIn = pInputs->FirstChildElement(); pIn != nullptr;
         pIn = pIn->NextSiblingElement())
      inputs.emplace_back(parseExpression(pIn, context, model));
  if (pOutputs)
    for (XMLElement *pOut = pOutputs->FirstChildElement(); pOut != nullptr;
         pOut = pOut->NextSiblingElement())
      outputs.emplace_back(parseExpression(pOut, context, model));
  OperationCall::shared_ptr call =
      belem::Factory::makeOperationCall(name, inputs, outputs);

  if (not operations_.contains(name)) throw IdentifierOutOfScope(name);
  Operation::shared_ptr operation = operations_[name];
  const vector<Ident::shared_ptr> &op_outputs = operation->getOutputParams();
  const vector<Ident::shared_ptr> &op_inputs = operation->getInputParams();

  // The identifier of the operation must have the same type as the operation
  // itself
  model->add(s_factory_.makeAssertEquals(name_id->getAssociatedVariable(),
                                         operation->getAssociatedVariable()));
  // Input args passed to the operation must have the same type as the one in
  // the definition
  for (unsigned int i = 0; i < inputs.size(); i++)
    model->add(
        s_factory_.makeAssertEquals(inputs[i]->getAssociatedVariable(),
                                    op_inputs[i]->getAssociatedVariable()));
  // Output args passed to the operation must have the same type as the one in
  // the definition
  for (unsigned int i = 0; i < outputs.size(); i++)
    model->add(
        s_factory_.makeAssertEquals(outputs[i]->getAssociatedVariable(),
                                    op_outputs[i]->getAssociatedVariable()));

  return call;
}

Loop::shared_ptr Parser::parseLoop(XMLElement *pLoop,
                                   Context::shared_ptr context,
                                   Model::shared_ptr model) {
  XMLElement *pCondition = pLoop->FirstChildElement("Condition");
  XMLElement *pBody = pLoop->FirstChildElement("Body");
  XMLElement *pInvariant = pLoop->FirstChildElement("Invariant");
  XMLElement *pVariant = pLoop->FirstChildElement("Variant");

  Predicate::shared_ptr condition =
      parsePredicate(pCondition->FirstChildElement(), context, model);
  Instruction::shared_ptr body =
      parseInstruction(pBody->FirstChildElement(), context, model);
  Predicate::shared_ptr invariant =
      parsePredicate(pInvariant->FirstChildElement(), context, model);
  Expression::shared_ptr variant =
      parseExpression(pVariant->FirstChildElement(), context, model);

  // Variant must be a integer
  model->add(s_factory_.makeAssertEquals(variant->getAssociatedVariable(),
                                         s_factory_.makeInteger()));

  return belem::Factory::makeLoop(condition, body, invariant, variant);
}

QuantifiedPred::shared_ptr Parser::parseQuantifiedPred(
    XMLElement *pPred, Context::shared_ptr context, Model::shared_ptr model) {
  string op = pPred->Attribute("type");
  XMLElement *pVariables = pPred->FirstChildElement("Variables");
  XMLElement *pBody = pPred->FirstChildElement("Body")->FirstChildElement();
  Context::shared_ptr new_context = context->copy_shared_ptr();
  vector<Expression::shared_ptr> variables;
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id"))
    variables.emplace_back(
        parseExpression(pVariable, new_context, model, false));
  Predicate::shared_ptr body = parsePredicate(pBody, new_context, model);
  QuantifiedPred::shared_ptr quantified_pred =
      belem::Factory::makeQuantifiedPred(op, variables, body);
  return quantified_pred;
}

BinaryPred::shared_ptr Parser::parseBinaryPred(XMLElement *pPred,
                                               Context::shared_ptr context,
                                               Model::shared_ptr model) {
  string op = pPred->Attribute("op");
  XMLElement *pLeft = pPred->FirstChildElement();
  if (pPred->FirstChildElement("Attr") != nullptr)
    pLeft = pPred->FirstChildElement("Attr")->NextSiblingElement();
  XMLElement *pRight = pLeft->NextSiblingElement();
  Predicate::shared_ptr left = parsePredicate(pLeft, context, model);
  Predicate::shared_ptr right = parsePredicate(pRight, context, model);
  BinaryPred::shared_ptr binary_pred =
      belem::Factory::makeBinaryPred(left, op, right);
  return binary_pred;
}

BinaryExp::shared_ptr Parser::parseBinaryExpression(XMLElement *pExp,
                                                    Context::shared_ptr context,
                                                    Model::shared_ptr model,
                                                    Position::shared_ptr pos) {
  string op = pExp->Attribute("op");
  XMLElement *pLeft = pExp->FirstChildElement();
  if (pExp->FirstChildElement("Attr") != nullptr)
    pLeft = pExp->FirstChildElement("Attr")->NextSiblingElement();
  XMLElement *pRight = pLeft->NextSiblingElement();
  Expression::shared_ptr left = parseExpression(pLeft, context, model);
  Expression::shared_ptr right = parseExpression(pRight, context, model);
  BinaryExp::shared_ptr bin_exp =
      belem::Factory::makeBinaryExp(left, op, right, pos);
  addExpression(model, bin_exp);
  if (op == "*s") {
    Variable::shared_ptr t1 = VarGenerator::getNewVariable();
    Variable::shared_ptr t2 = VarGenerator::getNewVariable();
    model->add(t1);
    model->add(t2);
    // The type of left must be of shape POW(t1)
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           s_factory_.makeBPow(t1)));
    // The type of right must be of shape POW(t2)
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(),
                                           s_factory_.makeBPow(t2)));
    // The type of the binary operation is t1 x t2 where POW(t1) POW(t2) are the
    // types of left and right
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(t1, t2))));
  } else if (op == "^") {
    // Both operands must be sequences
    Variable::shared_ptr t1 = addSequenceTypeConstraint(model, left);
    Variable::shared_ptr t2 = addSequenceTypeConstraint(model, right);
    // They also must have the same type
    model->add(s_factory_.makeAssertEquals(t1, t2));
    // The result will have the same type as the operands
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           left->getAssociatedVariable()));
  } else if (op == "->") {
    // The right operand must be a sequence
    Variable::shared_ptr t = addSequenceTypeConstraint(model, right);
    // The left operand must be of type t
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(), t));
    // The type of the expression is the same as the right operand
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           right->getAssociatedVariable()));
  } else if (op == "<-") {
    // The left operand must be a sequence
    Variable::shared_ptr t = addSequenceTypeConstraint(model, left);
    // The right operand must be of type t
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(), t));
    // The type of the expression is the same as the left operand
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           left->getAssociatedVariable()));
  } else if (op == "/|\\" or op == "\\|/") {
    // The left operand must be a sequence
    Variable::shared_ptr t = addSequenceTypeConstraint(model, left);
    // The right operand must be an integer
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
    // The type of the expression is the same as the left operand
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           left->getAssociatedVariable()));
  } else if (op == "..") {
    // Both operands should be integers
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
    // The type of the expression must be POW(INTEGER)
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeInteger())));
  } else if (op == "-s" or op == "/\\" or op == "\\/") {
    // Both operands must be sets
    addSetTypeConstraint(model, left);
    addSetTypeConstraint(model, right);
    // Both operands must have the same type
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           right->getAssociatedVariable()));
    // The expression must have the type of one of the operands
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           bin_exp->getAssociatedVariable()));
  } else if (op == "*i" or op == "**i" or op == "+i" or op == "-i" or
             op == "/i" or op == "mod") {
    // Right and left operand must have the same type
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           right->getAssociatedVariable()));
    // The type of the binary expression is the same as the operands
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           bin_exp->getAssociatedVariable()));
    // The type of the binary expression is integer
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
  } else if (op == "*r" or op == "**r" or op == "+r" or op == "-r" or
             op == "/r") {
    // Right and left operand must have the same type
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           right->getAssociatedVariable()));
    // The type of the binary expression is the same as the operands
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           bin_exp->getAssociatedVariable()));
    // The type of the binary expression is real
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           s_factory_.makeReal()));
  } else if (op == "*f" or op == "+f" or op == "-f" or op == "/f") {
    // Right and left operand must have the same type
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           right->getAssociatedVariable()));
    // The type of the binary expression is the same as the operands
    model->add(s_factory_.makeAssertEquals(left->getAssociatedVariable(),
                                           bin_exp->getAssociatedVariable()));
    // The type of the binary expression is float
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           s_factory_.makeBIdent("FLOAT")));
  } else if (op == "|->" or op == ",") {
    // The expression is of type T x U assuming the left and right operands are
    // of type T and U
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBCartesianProduct(left->getAssociatedVariable(),
                                         right->getAssociatedVariable())));
  } else if (op == "<->") {
    // Left and right operands must be sets
    Variable::shared_ptr t = addSetTypeConstraint(model, left);
    Variable::shared_ptr u = addSetTypeConstraint(model, right);
    // The expression must be of type POW(POW(t1 x t2))
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(
            s_factory_.makeBPow(s_factory_.makeBCartesianProduct(t, u)))));
  } else if (op == "prj1") {
    // Left and right operands must be sets
    Variable::shared_ptr t = addSetTypeConstraint(model, left);
    Variable::shared_ptr u = addSetTypeConstraint(model, right);
    // The expression must be of type POW(t x u x t)
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct({t, u, t}))));
  } else if (op == "prj2") {
    // Left and right operands must be sets
    Variable::shared_ptr t = addSetTypeConstraint(model, left);
    Variable::shared_ptr u = addSetTypeConstraint(model, right);
    // The expression must be of type POW(t x u x u)
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct({t, u, u}))));
  } else if (op == ";") {
    // Left operand must be of type POW(t x u1)
    auto [t, u1] = addRelationTypeConstraint(model, left);
    // Right operand should be of type POW(u2 x v)
    auto [u2, v] = addRelationTypeConstraint(model, right);
    // u1 and u2 must be the same
    model->add(s_factory_.makeAssertEquals(u1, u2));
    // The expression is of type POW(t x V)
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(t, v))));
  } else if (op == "><") {
    // The left operand must be of type POW(t1 x u)
    auto [t1, u] = addRelationTypeConstraint(model, left);
    // The right operand must be of type POW(t2 x v)
    const auto [t2, v] = addRelationTypeConstraint(model, right);
    // t1 and t2 must be equals
    model->add(s_factory_.makeAssertEquals(t1, t2));
    // The expression is of type POW(t1 x (u x v))
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(
            t1, s_factory_.makeBCartesianProduct(u, v)))));
  } else if (op == "||") {
    // The left operand must be of type POW(t x u)
    auto [t, u] = addRelationTypeConstraint(model, left);
    // The right operand must be of type POW(v x w)
    auto [v, w] = addRelationTypeConstraint(model, right);
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(
            s_factory_.makeBCartesianProduct(t, v),
            s_factory_.makeBCartesianProduct(u, w)))));
  } else if (op == "iterate") {
    // The left operand must be of type POW(T x T)
    auto [t, u] = addRelationTypeConstraint(model, left);
    model->add(s_factory_.makeAssertEquals(t, u));
    // The right operand must be an integer
    model->add(s_factory_.makeAssertEquals(right->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
    // The type of the expression is also POW(T x T)
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           left->getAssociatedVariable()));

  } else if (op == "[") {
    // The type of the left operand must be POW(t,u)
    auto [t1, u] = addRelationTypeConstraint(model, left);
    // The type of the right operand must be POW(t)
    Variable::shared_ptr t2 = addSetTypeConstraint(model, right);
    model->add(s_factory_.makeAssertEquals(t1, t2));
    // The expression is of type POW(u)
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           s_factory_.makeBPow(u)));

  } else if (op == "<<|" or op == "<|") {
    // Left operand must be a set of type POW(t)
    Variable::shared_ptr t1 = addSetTypeConstraint(model, left);
    // Right operand must be of type POW(t x u)
    auto [t2, u] = addRelationTypeConstraint(model, right);
    model->add(s_factory_.makeAssertEquals(t1, t2));
    // The expression is also of type POW(t x u)
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           right->getAssociatedVariable()));
  } else if (op == "|>" or op == "|>>") {
    // Left operand must be of type POW(t x u)
    auto [t, u1] = addRelationTypeConstraint(model, left);
    // Right operand must be of type POW(u)
    Variable::shared_ptr u2 = addSetTypeConstraint(model, right);
    model->add(s_factory_.makeAssertEquals(u1, u2));
    // The expression is also of type POW(t x u)
  } else if (op == "<+") {
    // Left and right operands must be of type POW(t x u)
    auto [t1, u1] = addRelationTypeConstraint(model, left);
    auto [t2, u2] = addRelationTypeConstraint(model, right);
    model->add(s_factory_.makeAssertEquals(t1, t2));
    model->add(s_factory_.makeAssertEquals(u1, u2));
    // The expression is also of type POW(t x u)
    model->add(s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(),
                                           left->getAssociatedVariable()));
  } else if (op == "-->" or op == "+->" or op == ">+>" or op == ">->" or
             op == "+->>" or op == "-->>" or op == ">->>") {
    // Left operand should be of type POW(t)
    Variable::shared_ptr t = addSetTypeConstraint(model, left);
    // Right operand should be of type POW(u)
    Variable::shared_ptr u = addSetTypeConstraint(model, right);
    // The expression is of type POW(POW(t x u))
    model->add(s_factory_.makeAssertEquals(
        bin_exp->getAssociatedVariable(),
        s_factory_.makeBPow(
            s_factory_.makeBPow(s_factory_.makeBCartesianProduct(t, u)))));
  } else if (op == "(") {
    // Left operand must be a function of type POW(t x u)
    auto [t, u] = addRelationTypeConstraint(model, left);
    // Right operand is of type t
    model->add(s_factory_.makeAssertEquals(t, right->getAssociatedVariable()));
    // The expression is of type u
    model->add(
        s_factory_.makeAssertEquals(bin_exp->getAssociatedVariable(), u));
  } else
    throw UnknownXmlElement("<Binary_Exp op=\"" + op + "\">");

  return bin_exp;
}

UnaryExp::shared_ptr Parser::parseUnaryExpression(XMLElement *pUnaryExp,
                                                  Context::shared_ptr context,
                                                  Model::shared_ptr model,
                                                  Position::shared_ptr pos) {
  string op = pUnaryExp->Attribute("op");
  XMLElement *pPos = pUnaryExp->FirstChildElement("Attr");
  XMLElement *pArg;
  if (pPos != nullptr)
    pArg = pPos->NextSiblingElement();
  else
    pArg = pUnaryExp->FirstChildElement();
  Expression::shared_ptr arg = parseExpression(pArg, context, model);
  UnaryExp::shared_ptr unary_exp = belem::Factory::makeUnaryExp(op, arg, pos);
  addExpression(model, unary_exp);
  if (op == "-i" or op == "succ" or op == "pred") {
    // For those operators, the argument must be an integer
    model->add(s_factory_.makeAssertEquals(arg->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
    // The type of the unary expression is the same as its argument
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           arg->getAssociatedVariable()));
  } else if (op == "real") {
    // The argument must be of type integer
    model->add(s_factory_.makeAssertEquals(arg->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
    // The result of the application is an real
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeReal()));
  } else if (op == "floor" or op == "ceiling") {
    // The argument must be of type real
    model->add(s_factory_.makeAssertEquals(arg->getAssociatedVariable(),
                                           s_factory_.makeReal()));
    // The result of the application is an integer
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
  } else if (op == "imax" or op == "imin") {
    // The argument must be a set of integers
    model->add(s_factory_.makeAssertEquals(
        arg->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeInteger())));
    // The result of the application is an integer
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
  } else if (op == "rmax" or op == "rmin") {
    // The argument must be a set of reals
    model->add(s_factory_.makeAssertEquals(
        arg->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeReal())));
    // The result of the application is a real
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeReal()));
  } else if (op == "card") {
    // The argument must be a set
    addSetTypeConstraint(model, arg);
    // The result of the application is an integer
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
  } else if (op == "POW" or op == "POW1" or op == "FIN" or op == "FIN1") {
    // The argument must be a set of type POW(t)
    Variable::shared_ptr t = addSetTypeConstraint(model, arg);
    // The type of the expression is POW(arg)
    model->add(s_factory_.makeAssertEquals(
        unary_exp->getAssociatedVariable(),
        s_factory_.makeBPow(arg->getAssociatedVariable())));
  } else if (op == "seq" or op == "seq1" or op == "iseq" or op == "iseq1" or
             op == "perm") {
    // Creating a fresh variable and adding it to the model
    Variable::shared_ptr var_type = VarGenerator::getNewVariable();
    model->add(var_type);
    // The argument of these operators must have a type of shape POW(T)
    model->add(s_factory_.makeAssertEquals(arg->getAssociatedVariable(),
                                           s_factory_.makeBPow(var_type)));
    // The type of the expression must be POW(POW(INTEGER x T))
    model->add(s_factory_.makeAssertEquals(
        unary_exp->getAssociatedVariable(),
        s_factory_.makeBPow(
            s_factory_.makeBPow(s_factory_.makeBCartesianProduct(
                s_factory_.makeInteger(), var_type)))));
  } else if (op == "size") {
    addSequenceTypeConstraint(model, arg);
    // The unary expression type must be INTEGER
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
  } else if (op == "first" or op == "last") {
    Variable::shared_ptr t = addSequenceTypeConstraint(model, arg);
    // The unary expression type must be t
    model->add(
        s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(), t));
  } else if (op == "front" or op == "tail" or op == "rev") {
    addSequenceTypeConstraint(model, arg);
    // The type of the unary expression must be the same as its argument
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           arg->getAssociatedVariable()));
  } else if (op == "conc") {
    Variable::shared_ptr t = VarGenerator::getNewVariable();
    // The argument must be of type POW(INTEGER x POW(INTEGER x t))
    model->add(s_factory_.makeAssertEquals(
        arg->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(
            s_factory_.makeInteger(),
            s_factory_.makeBPow(s_factory_.makeBCartesianProduct(
                s_factory_.makeInteger(), t))))));
    // The type of the unary expression must be POW(INTEGER, t)
    model->add(s_factory_.makeAssertEquals(
        unary_exp->getAssociatedVariable(),
        s_factory_.makeBPow(
            s_factory_.makeBCartesianProduct(s_factory_.makeInteger(), t))));

  } else if (op == "union" or op == "inter") {
    // The result must be a set of type POW(t)
    Variable::shared_ptr t = addSetTypeConstraint(model, unary_exp);
    // The argument sould be a set of set of type POW(POW(t))
    model->add(s_factory_.makeAssertEquals(
        arg->getAssociatedVariable(),
        s_factory_.makeBPow(unary_exp->getAssociatedVariable())));
  } else if (op == "id") {
    // The argument must be a set of type POW(t)
    Variable::shared_ptr t = addSetTypeConstraint(model, arg);
    // Thus, the expression is of type POW(t x t)
    model->add(s_factory_.makeAssertEquals(
        unary_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(t, t))));
  } else if (op == "~") {
    // The argument must be of type POW(t x u)
    Variable::shared_ptr t = VarGenerator::getNewVariable();
    Variable::shared_ptr u = VarGenerator::getNewVariable();
    model->add(u);
    model->add(t);
    model->add(s_factory_.makeAssertEquals(
        arg->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(t, u))));
    // The expression must be of type POW(u x t)
    model->add(s_factory_.makeAssertEquals(
        unary_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(u, t))));
  } else if (op == "closure" or op == "closure1") {
    // The argument must be of type POW(t x t)
    auto [t1, t2] = addRelationTypeConstraint(model, arg);
    model->add(s_factory_.makeAssertEquals(t1, t2));
    // The expression has the same type as the argument
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           arg->getAssociatedVariable()));
  } else if (op == "dom") {
    // The argument must be of type POW(t x u)
    auto [t, u] = addRelationTypeConstraint(model, arg);
    // The expression must be of type POW(t)
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeBPow(t)));
  } else if (op == "ran") {
    // The argument must be of type POW(t x u)
    auto [t, u] = addRelationTypeConstraint(model, arg);
    // The expression must be of type POW(u)
    model->add(s_factory_.makeAssertEquals(unary_exp->getAssociatedVariable(),
                                           s_factory_.makeBPow(u)));
  } else if (op == "fnc") {
    // The argument must be of type POW(t x u)
    auto [t, u] = addRelationTypeConstraint(model, arg);
    // The expression must be of type POW(t x POW(u))
    model->add(s_factory_.makeAssertEquals(
        unary_exp->getAssociatedVariable(),
        s_factory_.makeBPow(
            s_factory_.makeBCartesianProduct(t, s_factory_.makeBPow(u)))));
  } else if (op == "rel") {
    // The argument must be of type POW(t x POW(u))
    auto [t, pow_u] = addRelationTypeConstraint(model, arg);
    Variable::shared_ptr u = VarGenerator::getNewVariable();
    model->add(u);
    model->add(s_factory_.makeAssertEquals(pow_u, s_factory_.makeBPow(u)));
    // The type of the expression in POW(t x u)
    model->add(s_factory_.makeAssertEquals(
        unary_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(t, u))));
  } else
    throw UnknownXmlElement("<Unary_Exp op=\"" + op + "\">");

  return unary_exp;
}

BooleanExpression::shared_ptr Parser::parseBooleanExpression(
    XMLElement *pBool, Context::shared_ptr context, Model::shared_ptr model,
    Position::shared_ptr position) {
  XMLElement *pPos = pBool->FirstChildElement("Attr");
  XMLElement *pPred;
  if (pPos != nullptr)
    pPred = pPos->NextSiblingElement();
  else
    pPred = pBool->FirstChildElement();
  Predicate::shared_ptr pred = parsePredicate(pPred, context, model);
  BooleanExpression::shared_ptr bool_exp =
      belem::Factory::makeBooleanExp(pred, position);
  addExpression(model, bool_exp);
  // The result of the application is a bool
  model->add(s_factory_.makeAssertEquals(bool_exp->getAssociatedVariable(),
                                         s_factory_.makeBool()));
  return bool_exp;
}

NaryExp::shared_ptr Parser::parseNaryExpression(XMLElement *pExp,
                                                Context::shared_ptr context,
                                                Model::shared_ptr model,
                                                Position::shared_ptr position) {
  string op = pExp->Attribute("op");
  vector<Expression::shared_ptr> operands;
  XMLElement *pPos = pExp->FirstChildElement("Attr");
  XMLElement *pChild;
  if (pPos != nullptr)
    pChild = pPos->NextSiblingElement();
  else
    pChild = pExp->FirstChildElement();
  for (XMLElement *pOperand = pChild; pOperand != nullptr;
       pOperand = pOperand->NextSiblingElement())
    operands.emplace_back(parseExpression(pOperand, context, model));

  NaryExp::shared_ptr nary_exp =
      belem::Factory::makeNaryExp(operands, op, position);
  addExpression(model, nary_exp);
  if (operands.size() != 0) {
    Variable::shared_ptr type = operands[0]->getAssociatedVariable();
    for (unsigned int i = 1; i < operands.size(); i++) {
      // The elements in the sequence or set must have the same type
      model->add(s_factory_.makeAssertEquals(
          type, operands[i]->getAssociatedVariable()));
    }
    if (op == "{")
      // For a set, the type of the expression is POW(T), assuming its elements
      // are of type T
      model->add(s_factory_.makeAssertEquals(nary_exp->getAssociatedVariable(),
                                             s_factory_.makeBPow(type)));
    if (op == "[")
      // For a sequence, the type of the expression is POW(INTEGER x T) assuming
      // its elements are of type T
      model->add(s_factory_.makeAssertEquals(
          nary_exp->getAssociatedVariable(),
          s_factory_.makeBPow(s_factory_.makeBCartesianProduct(
              s_factory_.makeInteger(), type))));
  }

  return nary_exp;
}

NaryExp::shared_ptr Parser::parseEmptySeq(Context::shared_ptr context,
                                          Model::shared_ptr model,
                                          Position::shared_ptr pos) {
  NaryExp::shared_ptr empty_seq = belem::Factory::makeNaryExp({}, "[", pos);
  addExpression(model, empty_seq);
  return empty_seq;
}

NaryExp::shared_ptr Parser::parseEmptySet(Context::shared_ptr context,
                                          Model::shared_ptr model,
                                          Position::shared_ptr pos) {
  NaryExp::shared_ptr empty_set = belem::Factory::makeNaryExp({}, "{", pos);
  addExpression(model, empty_set);
  return empty_set;
}

QuantifiedSet::shared_ptr Parser::parseQuantifiedSet(
    XMLElement *pSet, Context::shared_ptr context, Model::shared_ptr model,
    Position::shared_ptr pos) {
  XMLElement *pVariables = pSet->FirstChildElement("Variables");
  XMLElement *pBody = pSet->FirstChildElement("Body");
  vector<Expression::shared_ptr> variables;
  Context::shared_ptr new_context = context->copy_shared_ptr();
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id"))
    variables.emplace_back(
        parseExpression(pVariable, new_context, model, false));
  Predicate::shared_ptr predicate =
      parsePredicate(pBody->FirstChildElement(), new_context, model);
  QuantifiedSet::shared_ptr quantified_set =
      belem::Factory::makeQuantifiedSet(variables, predicate, pos);
  addExpression(model, quantified_set);
  // The type of the set is POW(t1 x ... x tn) assuming the expressions are of
  // type t1, ..., tn
  solver::AbstractBType::shared_ptr type;
  if (variables.size() == 1)
    type = variables[0]->getAssociatedVariable();
  else {
    vector<solver::AbstractBType::shared_ptr> types;
    for (auto &&expression : variables)
      types.emplace_back(expression->getAssociatedVariable());
    type = s_factory_.makeBCartesianProduct(types);
  }
  model->add(s_factory_.makeAssertEquals(
      quantified_set->getAssociatedVariable(), s_factory_.makeBPow(type)));
  return quantified_set;
}

QuantifiedExp::shared_ptr Parser::parseQuantifiedExp(
    XMLElement *pExp, Context::shared_ptr context, Model::shared_ptr model,
    Position::shared_ptr pos) {
  string type = pExp->Attribute("type");
  XMLElement *pVariables = pExp->FirstChildElement("Variables");
  XMLElement *pPred = pExp->FirstChildElement("Pred")->FirstChildElement();
  XMLElement *pBody = pExp->FirstChildElement("Body")->FirstChildElement();
  Context::shared_ptr new_context = context->copy_shared_ptr();
  vector<Expression::shared_ptr> variables;
  for (XMLElement *pVariable = pVariables->FirstChildElement("Id");
       pVariable != nullptr; pVariable = pVariable->NextSiblingElement("Id"))
    variables.emplace_back(
        parseExpression(pVariable, new_context, model, false));
  Predicate::shared_ptr pred = parsePredicate(pPred, new_context, model);
  Expression::shared_ptr body = parseExpression(pBody, new_context, model);
  QuantifiedExp::shared_ptr quantified_exp =
      belem::Factory::makeQuantifiedExp(type, variables, pred, body, pos);
  addExpression(model, quantified_exp);
  if (type == "iSIGMA" or type == "iPI") {
    // The type of the expression must be integer
    model->add(s_factory_.makeAssertEquals(
        quantified_exp->getAssociatedVariable(), s_factory_.makeInteger()));
    // The type of the body must be integer
    model->add(s_factory_.makeAssertEquals(body->getAssociatedVariable(),
                                           s_factory_.makeInteger()));
  } else if (type == "rSIGMA" or type == "rPI") {
    // The type of the expression must be real
    model->add(s_factory_.makeAssertEquals(
        quantified_exp->getAssociatedVariable(), s_factory_.makeReal()));
    // The type of the body must be real
    model->add(s_factory_.makeAssertEquals(body->getAssociatedVariable(),
                                           s_factory_.makeReal()));
  } else if (type == "UNION" or type == "INTER") {
    // The type of the expression must be a set
    addSetTypeConstraint(model, quantified_exp);
    // The body must have the same type as the expression
    model->add(
        s_factory_.makeAssertEquals(quantified_exp->getAssociatedVariable(),
                                    body->getAssociatedVariable()));
  } else if (type == "%") {
    vector<solver::AbstractBType::shared_ptr> types;
    // The type of the expression must be POW(t1, ..., tn, t)
    for (auto &&var : variables)
      types.emplace_back(var->getAssociatedVariable());
    types.emplace_back(body->getAssociatedVariable());
    model->add(s_factory_.makeAssertEquals(
        quantified_exp->getAssociatedVariable(),
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(types))));

  } else
    throw UnknownXmlElement("<Quantified_Exp type=\"" + type + "\">");

  return quantified_exp;
}

Valuation::shared_ptr Parser::parseValuation(XMLElement *pValuation,
                                             Context::shared_ptr context,
                                             Model::shared_ptr model,
                                             Position::shared_ptr pos) {
  XMLElement *pPos = pValuation->FirstChildElement("Attr");
  XMLElement *pVal;
  if (pPos != nullptr)
    pVal = pPos->NextSiblingElement();
  else
    pVal = pValuation->FirstChildElement();
  Expression::shared_ptr var = context->get(pValuation->Attribute("ident"));
  Expression::shared_ptr val = parseExpression(pVal, context, model);
  Valuation::shared_ptr valuation =
      belem::Factory::makeValuation(var, val, pos);
  addExpression(model, valuation);
  // The type of the valuation is the same as its value and variable
  model->add(s_factory_.makeAssertEquals(valuation->getAssociatedVariable(),
                                         val->getAssociatedVariable()));
  model->add(s_factory_.makeAssertEquals(valuation->getAssociatedVariable(),
                                         var->getAssociatedVariable()));
  return valuation;
}

Operation::shared_ptr Parser::parseOperation(XMLElement *pOp,
                                             Context::shared_ptr context,
                                             Model::shared_ptr model) {
  XMLElement *pOutputs = pOp->FirstChildElement("Output_Parameters");
  XMLElement *pInputs = pOp->FirstChildElement("Input_Parameters");
  XMLElement *pPrecondition = pOp->FirstChildElement("Precondition");
  XMLElement *pBody = pOp->FirstChildElement("Body");
  string name = prefix_ + pOp->Attribute("name");
  std::vector<Ident::shared_ptr> inputs, outputs;
  Predicate::shared_ptr precondition;
  Instruction::shared_ptr body;
  Context::shared_ptr local_context = context->copy_shared_ptr();
  if (pOutputs != nullptr)
    for (XMLElement *pId = pOutputs->FirstChildElement("Id"); pId != nullptr;
         pId = pId->NextSiblingElement("Id")) {
      outputs.emplace_back(parseExpression(pId, local_context, model));
    }
  if (pInputs != nullptr)
    for (XMLElement *pId = pInputs->FirstChildElement("Id"); pId != nullptr;
         pId = pId->NextSiblingElement("Id")) {
      inputs.emplace_back(parseExpression(pId, local_context, model));
    }
  if (pPrecondition != nullptr)
    precondition = parsePredicate(pPrecondition->FirstChildElement(),
                                  local_context, model);
  if (pBody != nullptr)
    body = parseInstruction(pBody->FirstChildElement(), local_context, model);
  vector<AbstractBType::shared_ptr> args_types;
  for (auto &&input : inputs)
    args_types.emplace_back(input->getAssociatedVariable());
  for (auto &&output : outputs)
    args_types.emplace_back(output->getAssociatedVariable());
  Variable::shared_ptr var = VarGenerator::getNewVariable();
  model->add(var);
  Operation::shared_ptr operation = belem::Factory::makeOperation(
      name, inputs, outputs, precondition, body, var);
  // The operation is of type POW(Input x Output)
  if (args_types.size() == 1)
    model->add(s_factory_.makeAssertEquals(var, args_types[0]));
  else if (args_types.size() > 1)
    model->add(s_factory_.makeAssertEquals(
        var,
        s_factory_.makeBPow(s_factory_.makeBCartesianProduct(args_types))));
  // If the operation is refined
  if (operations_.contains(name)) {
    vector<Expression::shared_ptr> abstraction_inputs =
        operations_[name]->getInputParams();
    vector<Expression::shared_ptr> abstraction_outputs =
        operations_[name]->getOutputParams();
    if (inputs.size() != abstraction_inputs.size())
      throw ParserError("Operation " + name +
                        " does not have the same number of arguments in its "
                        "abstraction and its refinment");
    // Abstraction inputs should have the same type as the refinment inputs
    for (unsigned int i = 0; i < inputs.size(); i++)
      model->add(s_factory_.makeAssertEquals(
          inputs[i]->getAssociatedVariable(),
          abstraction_inputs[i]->getAssociatedVariable()));
    // Abstraction outputs should have the same type as the refinment outputs
    for (unsigned int i = 0; i < outputs.size(); i++)
      model->add(s_factory_.makeAssertEquals(
          outputs[i]->getAssociatedVariable(),
          abstraction_outputs[i]->getAssociatedVariable()));
  }
  operations_[name] = operation;
  return operation;
}

void Parser::parseOperations(XMLElement *pOps, Context::shared_ptr context,
                             Model::shared_ptr model) {
  if (pOps == nullptr) return;
  for (XMLElement *pOp = pOps->FirstChildElement("Operation"); pOp != nullptr;
       pOp = pOp->NextSiblingElement("Operation"))
    parseOperation(pOp, context, model);
  for (XMLElement *pOp = pOps->FirstChildElement("ImplicitOperation");
       pOp != nullptr; pOp = pOp->NextSiblingElement("ImplicitOperation"))
    parseOperation(pOp, context, model);
}

Variable::shared_ptr Parser::addSequenceTypeConstraint(
    Model::shared_ptr model, Expression::shared_ptr sequence) {
  // If the expression already has been marked as a sequence, there is no
  // need to add another constraint
  if (sequences_.contains(sequence)) {
    return sequences_[sequence];
  }
  // Adding a fresh type t
  Variable::shared_ptr t = VarGenerator::getNewVariable();
  model->add(t);
  // The argument must be of type POW(INTEGER x t)
  model->add(s_factory_.makeAssertEquals(
      sequence->getAssociatedVariable(),
      s_factory_.makeBPow(
          s_factory_.makeBCartesianProduct(s_factory_.makeInteger(), t))));
  sequences_[sequence] = t;
  return t;
}

Variable::shared_ptr Parser::addSetTypeConstraint(Model::shared_ptr model,
                                                  Expression::shared_ptr set) {
  // If the expression already has been marked as a set, there is no
  // need to add another constraint
  if (sets_.contains(set)) {
    return dynamic_pointer_cast<Variable>(sets_[set]->getType());
  }
  // Adding a fresh type t
  Variable::shared_ptr t = VarGenerator::getNewVariable();
  model->add(t);
  // The argument must be of type POW(t)
  BPow::shared_ptr pow = s_factory_.makeBPow(t);
  model->add(s_factory_.makeAssertEquals(set->getAssociatedVariable(), pow));
  sets_[set] = pow;
  return t;
}

pair<Variable::shared_ptr, Variable::shared_ptr>
Parser::addRelationTypeConstraint(Model::shared_ptr model,
                                  Expression::shared_ptr relation) {
  // If the expression already has been marked as a relation, there is no
  // need to add another constraint
  if (relations_.contains(relation)) {
    BCartesianProduct::shared_ptr product = relations_[relation];
    Variable::shared_ptr t = dynamic_pointer_cast<Variable>(product->getLeft());
    Variable::shared_ptr u =
        dynamic_pointer_cast<Variable>(product->getRight());
    return {t, u};
  }
  // Adding the fresh types t and u
  Variable::shared_ptr t = VarGenerator::getNewVariable();
  Variable::shared_ptr u = VarGenerator::getNewVariable();
  model->add(t);
  model->add(u);
  // The argument must be of type POW(t x u)
  BCartesianProduct::shared_ptr product =
      s_factory_.makeBCartesianProduct(t, u);
  model->add(s_factory_.makeAssertEquals(relation->getAssociatedVariable(),
                                         s_factory_.makeBPow(product)));
  relations_[relation] = product;
  return {t, u};
}

}  // namespace genericparser
