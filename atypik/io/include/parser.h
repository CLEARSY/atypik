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
#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <set>
#include <string>
#include <tinyxml2.h>

#include "belemfactory.h"
#include "bxmlposition.h"
#include "context.h"
#include "machinetypes.h"
#include "solverfactory.h"

#include <iostream>

namespace genericparser {

class UnknownXmlElement : public std::exception
{
public:
    /*!
     * \brief Construct an UnknownBxmlElement from a message
     * \param msg
     * The message
     */
    UnknownXmlElement(std::string msg);
    const char * what() const noexcept override;

private:
    std::string message_;
};

class ParserError : public std::exception
{
public:
    /*!
     * \brief Construct a ParserError from a message
     * \param msg
     * The message
     */
    ParserError(std::string msg);
    const char * what() const noexcept override;

private:
    std::string message_;
};

class Parser
{
public:
    /*!
     * \brief A unique_ptr on a Parser
     */
    typedef std::unique_ptr<Parser> unique_ptr;
    /*!
     * \brief Parse a bxml file and extract types variables and constraints from it
     * \param pDocument
     * The document to parse
     * \return a Context of parsing
     */
    virtual Context::shared_ptr parse(tinyxml2::XMLDocument *pDocument) = 0;
    /*!
     * \brief Disable multi threading for pog files
     */
    void disableMultiThread();


protected:
    /*!
     * \brief A boolean telling if multi thread is enable or not
     */
    bool enable_multi_thread_ = true;
    /*!
     * \brief The global context containing the identifiers in the sets
     */
    Context::shared_ptr global_context_ = std::make_shared<Context>(true);
    /*!
     * \brief The expressions marked as sequences associated to their type
     */
    std::unordered_map<belem::Expression::shared_ptr, solver::Variable::shared_ptr> sequences_;
    /*!
     * \brief The expressions marked as sets associated to their type
     */
    std::unordered_map<belem::Expression::shared_ptr, solver::BPow::shared_ptr> sets_;
    /*!
     * \brief The expressions marked as relations associated to their type
     */
    std::unordered_map<belem::Expression::shared_ptr, solver::BCartesianProduct::shared_ptr> relations_;
    /*!
     * \brief The parsed expressions
     */
    std::unordered_set<belem::Expression::shared_ptr> expressions_;
    /*!
     * \brief The parsed operations associated to their name
     */
    std::unordered_map<std::string, belem::Operation::shared_ptr> operations_;
    /*!
     * \brief An instance of the solver factory
     */
    solver::Factory s_factory_;
    /*!
     * \brief The prefix to add to the identifiers (if the parsed machine is
     * referenced and instanciated)
     */
    std::string prefix_;
    /*!
     * \brief The type of the machine
     */
    genericparser::MachineType type_;
    /*!
     * \brief Add an expression into the parsed expressions and declaring its
     * type in the problrem
     * \param Model
     * The Model to modify
     * \param expression
     * The expression to add
     */
    void addExpression(solver::Model::shared_ptr model,
                       belem::Expression::shared_ptr expression);
    /*!
     * \brief Initialize a model with B default types
     * \return a shared pointer on the initialiazed model
     */
    solver::Model::shared_ptr initModel();
    /*!
     * \brief Parse a position from an Attr tag
     * \param pAttr
     * A pointer on the Attr tag
     * \param pElement
     * The tag representing the expression linked to the position
     * \return the position in the tag. If the tag doesn't contain a position,
     * return the position l=-1, c=-1, s=-1
     */
    bxml::Position::shared_ptr getPosition(tinyxml2::XMLElement * pAttr, tinyxml2::XMLElement *pElement);
    /*!
     * \brief Parse an Expression in a XMLElement and extracts types variables and
     * constrainst from it
     * \param pExpression
     * A pointer on the XMLElement
     * \param context
     * The current context
     * \param Model
     * The input Model
     * \param lookup_in_context
     * A boolean telling if the id must be search in the context
     * \return the parsed expression
     */
    belem::Expression::shared_ptr parseExpression(tinyxml2::XMLElement * pExpression,
                                                  genericparser::Context::shared_ptr context,
                                                  solver::Model::shared_ptr model,
                                                  bool lookup_in_context = true);
    /*!
     * \brief Parse an Instruction in a XMLElement and extracts types variables and
     * constrainst from it
     * \param pInstruction
     * A pointer on the XMLElement
     * \param context
     * The current context
     * \param Model
     * The input Model
     * \return the parsed instruction
     */
    belem::Instruction::shared_ptr parseInstruction(tinyxml2::XMLElement * pInstruction,
                                                    genericparser::Context::shared_ptr context,
                                                    solver::Model::shared_ptr model);
    /*!
     * \brief Parse a Predicate in a XMLElement and extracts types variables and
     * constrainst from it
     * \param pPredicate
     * A pointer on the XMLElement
     * \param context
     * The current context
     * \param Model
     * The input Model
     * \return the parsed predicate
     */
    belem::Predicate::shared_ptr parsePredicate(tinyxml2::XMLElement * pPredicate,
                                                genericparser::Context::shared_ptr context,
                                                solver::Model::shared_ptr model);
    /*!
     * \brief Push identifier contained in a given tag into a context
     * \param pIdentifiers
     * A pointer on the tag
     * \param context
     * The context
     * \param Model
     * The input Model
     */
    void parseIdentifiers(tinyxml2::XMLElement * pIdentifiers,
                          genericparser::Context::shared_ptr context,
                          solver::Model::shared_ptr model);
    /*!
     * \brief Parse the predicates in a tag
     * \param pPredicates
     * A pointer on the tag
     * \param context
     * The context
     * \param Model
     * The input Model
     */
    void parsePredicates(tinyxml2::XMLElement * pPredicates,
                         genericparser::Context::shared_ptr context,
                         solver::Model::shared_ptr model);
    /*!
     * \brief Parse the parameters tag
     * \param pParameters
     * A pointer on the parameters tag
     * \param context
     * The context
     * \param model
     * The input model
     * \param type
     * The type of the machine
     */
    void parseParameters(tinyxml2::XMLElement * pParameters,
                         genericparser::Context::shared_ptr context,
                         solver::Model::shared_ptr model,
                         MachineType type);
    /*!
     * \brief Parse an initialisation tag
     * \param pInitialisation
     * A pointer on the initialisation tag
     * \param context
     * The context
     * \param Model
     * The input Model
     */
    void parseInitialisation(tinyxml2::XMLElement * pInitialisation,
                             genericparser::Context::shared_ptr context,
                             solver::Model::shared_ptr model);
    /*!
     * \brief Parse the sets tag and add a new type for each set in the
     * context
     * \param pSets
     * A pointer on the sets tag
     * \param context
     * The context
     * \param Model
     * The Model
     * \param type
     * The type of the parsed machine. If it is an implementation, the
     * type of the sets becomes POW(INTEGER)
     */
    void parseSets(tinyxml2::XMLElement * pSets,
                   genericparser::Context::shared_ptr context,
                   solver::Model::shared_ptr model,
                   MachineType type);
    /*!
     * \brief Parse the valuations of the sets in a tag
     * \param pValues
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     */
    void parseValues(tinyxml2::XMLElement * pValues,
                     genericparser::Context::shared_ptr context,
                     solver::Model::shared_ptr model);
    /*!
     * \brief Parse the expressions in a tag
     * \param pExpressions
     * A pointer on a tag containing multiple expressions
     * \param context
     * The context
     * \param Model
     * The input Model
     * \return a vector of expressions
     */
    std::vector<belem::Expression::shared_ptr> parseExpressions(tinyxml2::XMLElement * pExpressions,
                                                                genericparser::Context::shared_ptr context,
                                                                solver::Model::shared_ptr model);
    /*!
     * \brief Parse the identifier in a tag
     * \param pId
     * A pointer on a tag containing an identifier
     * \param context
     * The context
     * \param Model
     * The input Model
     * \param pos
     * The position of the identifier
     * \param lookup_in_context
     * A boolean telling if the id must be search in the context
     * \return the parsed identifier
     */
    belem::Ident::shared_ptr parseId(tinyxml2::XMLElement * pId,
                                     genericparser::Context::shared_ptr context,
                                     solver::Model::shared_ptr model,
                                     bxml::Position::shared_ptr pos,
                                     bool lookup_in_context = true);
    /*!
     * \brief Parse a default type using its name
     * \param pTag
     * A pointer on the tag to parse
     * \param context
     * The context
     * \param Model
     * The input Model
     * \param type_name
     * The name of the default type
     * \param pos
     * The position of the expression
     * \return the parsed default type expression
     */
    belem::Expression::shared_ptr parseDefaultType(tinyxml2::XMLElement * pTag,
                                                   genericparser::Context::shared_ptr context,
                                                   solver::Model::shared_ptr model,
                                                   std::string type_name,
                                                   bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse a set from a tag
     * \param pSet
     * A pointer on the tag
     * \param contex
     * The context
     * \param Model
     * The input Model
     * \param pos
     * The position of the set
     * \param type
     * The type of the parsed machine
     * \return the parsed set
     */
    belem::Set::shared_ptr parseSet(tinyxml2::XMLElement * pSet,
                                    genericparser::Context::shared_ptr context,
                                    solver::Model::shared_ptr model,
                                    bxml::Position::shared_ptr pos,
                                    MachineType type);
    /*!
     * \brief Parse a n-arity substitution from a tag
     * \param pSub
     * A pointer on the tag
     * \param context
     * The context
     * \param Model
     * The input Model
     * \return the parsed n-arity substitution
     */
    belem::NarySub::shared_ptr parseNarySub(tinyxml2::XMLElement * pSub,
                                            genericparser::Context::shared_ptr context,
                                            solver::Model::shared_ptr model);
    /*!
     * \brief Parse assignments from a tag
     * \param pAssignments
     * A pointer on the tag
     * \param context
     * The context
     * \param Model
     * The input Model
     * \return the parsed Assignments
     */
    belem::Assignments::shared_ptr parseAssignments(tinyxml2::XMLElement * pAssignments,
                                                    genericparser::Context::shared_ptr context,
                                                    solver::Model::shared_ptr model);
    /*!
     * \brief Parse a block from a tag
     * \param pBlock
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The input model
     * \return the parsed block
     */
    belem::Block::shared_ptr parseBlock(tinyxml2::XMLElement * pBlock,
                                        genericparser::Context::shared_ptr context,
                                        solver::Model::shared_ptr model);
    /*!
     * \brief Parse an unary predicate from a tag
     * \param pPred
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed unary predicate
     */
    belem::UnaryPred::shared_ptr parseUnaryPred(tinyxml2::XMLElement * pPred,
                                                genericparser::Context::shared_ptr context,
                                                solver::Model::shared_ptr model);
    /*!
     * \brief Parse a n-arity predicate from a tag
     * \param pPred
     * A pointer on the tag
     * \param context
     * The context
     * \param Model
     * The input Model
     * \return the parsed n-arity predicate
     */
    belem::NaryPred::shared_ptr parseNaryPred(tinyxml2::XMLElement * pPred,
                                              genericparser::Context::shared_ptr context,
                                              solver::Model::shared_ptr model);
    /*!
     * \brief Parse a comparison from a tag
     * \param pPred
     * A pointer on the tag
     * \param context
     * The context
     * \param Model
     * The input Model
     * \return the parsed comparison
     */
    belem::Comparison::shared_ptr parseComparison(tinyxml2::XMLElement * pComp,
                                                  genericparser::Context::shared_ptr context,
                                                  solver::Model::shared_ptr model);
    /*!
     * \brief Parse an assertion from a tag
     * \param pAssertion
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed assertion
     */
    belem::Assert::shared_ptr parseAssertion(tinyxml2::XMLElement * pAssertion,
                                             genericparser::Context::shared_ptr context,
                                             solver::Model::shared_ptr model);
    /*!
     * \brief Parse an if from a tag
     * \param pIf
     * A pointer on the if
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed if
     */
    belem::Conditional::shared_ptr parseIf(tinyxml2::XMLElement * pIf,
                                           genericparser::Context::shared_ptr context,
                                           solver::Model::shared_ptr model);
    /*!
     * \brief Parse a select from a tag
     * \param pSelect
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed select
     */
    belem::Select::shared_ptr parseSelect(tinyxml2::XMLElement * pSelect,
                                          genericparser::Context::shared_ptr context,
                                          solver::Model::shared_ptr model);
    /*!
     * \brief Parse a case from a tag
     * \param pCase
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed case
     */
    belem::Case::shared_ptr parseCase(tinyxml2::XMLElement * pCase,
                                      genericparser::Context::shared_ptr context,
                                      solver::Model::shared_ptr model);
    /*!
     * \brief Parse a any instruction from a tag
     * \param pAny
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed any instruction
     */
    belem::Any::shared_ptr parseAny(tinyxml2::XMLElement * pAny,
                                    genericparser::Context::shared_ptr context,
                                    solver::Model::shared_ptr model);
    /*!
     * \brief Parse a let instruction from a tag
     * \param pLet
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed let instruction
     */
    belem::Let::shared_ptr parseLet(tinyxml2::XMLElement * pLet,
                                    genericparser::Context::shared_ptr context,
                                    solver::Model::shared_ptr model);
    /*!
     * \brief Parse a becomes in instruction from a tag
     * \param pBecomes
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed becomes in instruction
     */
    belem::BecomesIn::shared_ptr parseBecomesIn(tinyxml2::XMLElement * pBecomes,
                                                genericparser::Context::shared_ptr context,
                                                solver::Model::shared_ptr model);
    /*!
     * \brief Parse a becomes such that instruction from a tag
     * \param pBecomes
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed becomes such that instruction
     */
    belem::BecomesSuchThat::shared_ptr parseBecomesSuchThat(tinyxml2::XMLElement * pBecomes,
                                                            genericparser::Context::shared_ptr context,
                                                            solver::Model::shared_ptr model);
    /*!
     * \brief Parse a var in instruction from a tag
     * \param pVarIn
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed var in instruction
     */
    belem::VarIn::shared_ptr parseVarIn(tinyxml2::XMLElement * pVarIn,
                                        genericparser::Context::shared_ptr context,
                                        solver::Model::shared_ptr model);
    /*!
     * \brief Parse an operation call from a tag
     * \param pCall
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed operation call
     */
    belem::OperationCall::shared_ptr parseOperationCall(tinyxml2::XMLElement * pCall,
                                                        genericparser::Context::shared_ptr context,
                                                        solver::Model::shared_ptr model);
    /*!
     * \brief Parse a while instruction from a tag
     * \param pLoop
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed loop
     */
    belem::Loop::shared_ptr parseLoop(tinyxml2::XMLElement * pLoop,
                                      genericparser::Context::shared_ptr context,
                                      solver::Model::shared_ptr model);
    /*!
     * \brief Parse a quantified predicate from a tag
     * \param pPred
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The input model
     * \return the parsed quantified predicate
     */
    belem::QuantifiedPred::shared_ptr parseQuantifiedPred(tinyxml2::XMLElement * pPred,
                                                          genericparser::Context::shared_ptr context,
                                                          solver::Model::shared_ptr model);
    /*!
     * \brief Parse a binary predicate from a tag
     * \param pPred
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The input model
     * \return the parsed binary predicate
     */
    belem::BinaryPred::shared_ptr parseBinaryPred(tinyxml2::XMLElement *pPred,
                                                  genericparser::Context::shared_ptr context,
                                                  solver::Model::shared_ptr model);
    /*!
     * \brief Parse a binary expression from a tag
     * \param pBinExp
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed binary operation
     */
    belem::BinaryExp::shared_ptr parseBinaryExpression(tinyxml2::XMLElement * pBinExp,
                                                       genericparser::Context::shared_ptr context,
                                                       solver::Model::shared_ptr model,
                                                       bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse an unary expression from a tag
     * \param pUnaryExp
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed unary operation
     */
    belem::BinaryExp::shared_ptr parseUnaryExpression(tinyxml2::XMLElement * pUnaryExp,
                                                      genericparser::Context::shared_ptr context,
                                                      solver::Model::shared_ptr model,
                                                      bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse a boolean expression from a tag
     * \param pBool
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed boolean expression
     */
    belem::BooleanExpression::shared_ptr parseBooleanExpression(tinyxml2::XMLElement * pBool,
                                                                genericparser::Context::shared_ptr context,
                                                                solver::Model::shared_ptr model,
                                                                bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse an expression of arity n from a tag
     * \param pExp
     * The tag
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed expression
     */
    belem::NaryExp::shared_ptr parseNaryExpression(tinyxml2::XMLElement * pExp,
                                                   genericparser::Context::shared_ptr context,
                                                   solver::Model::shared_ptr model,
                                                   bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse an empty sequence
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed empty sequence
     */
    belem::NaryExp::shared_ptr parseEmptySeq(genericparser::Context::shared_ptr context,
                                             solver::Model::shared_ptr model,
                                             bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse an empty set
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed empty set
     */
    belem::NaryExp::shared_ptr parseEmptySet(genericparser::Context::shared_ptr context,
                                             solver::Model::shared_ptr model,
                                             bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse a quantified set from a tag
     * \param pSet
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed quantified set
     */
    belem::QuantifiedSet::shared_ptr parseQuantifiedSet(tinyxml2::XMLElement * pSet,
                                                        genericparser::Context::shared_ptr context,
                                                        solver::Model::shared_ptr model,
                                                        bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse a quantified expression from a tag
     * \param pExp
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed quantified expression
     */
    belem::QuantifiedExp::shared_ptr parseQuantifiedExp(tinyxml2::XMLElement * pExp,
                                                        genericparser::Context::shared_ptr context,
                                                        solver::Model::shared_ptr model,
                                                        bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse a valuation from a tag
     * \param pValuation
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \param pos
     * The position in the bxml file
     * \return the parsed valuation
     */
    belem::Valuation::shared_ptr parseValuation(tinyxml2::XMLElement * pValuation,
                                                genericparser::Context::shared_ptr context,
                                                solver::Model::shared_ptr model,
                                                bxml::Position::shared_ptr pos);
    /*!
     * \brief Parse an operation from a tag
     * \param pOp
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     * \return the parsed operation
     */
    belem::Operation::shared_ptr parseOperation(tinyxml2::XMLElement * pOp,
                                                genericparser::Context::shared_ptr context,
                                                solver::Model::shared_ptr model);
    /*!
     * \brief Parse the operations from a tag
     * \param pOps
     * A pointer on the tag
     * \param context
     * The context
     * \param model
     * The model
     */
    void parseOperations(tinyxml2::XMLElement * pOps,
                         genericparser::Context::shared_ptr context,
                         solver::Model::shared_ptr model);
    /*!
     * \brief Add a constraint on an expression to ensure its type is of
     * shape POW(INTEGER, T)
     * \param model
     * The model
     * \param sequence
     * The expression on which to add the constraint
     * \return a fresh variable representing the type T
     */
    solver::Variable::shared_ptr addSequenceTypeConstraint(solver::Model::shared_ptr model,
                                                           belem::Expression::shared_ptr sequence);
    /*!
     * \brief Add a constraint on an expression to ensure its type is of
     * shape POW(T)
     * \param model
     * The model
     * \param set
     * The expression on which to add the constraint
     * \return a fresh variable representing the type T
     */
    solver::Variable::shared_ptr addSetTypeConstraint(solver::Model::shared_ptr model,
                                                      belem::Expression::shared_ptr set);
    /*!
     * \brief Add a constraint on an expression to ensure its type is of
     * shape POW(T, U)
     * \param model
     * The model
     * \param relation
     * The expression on which to add the constraint
     * \return a pair of fresh variables representing T and U
     */
    std::pair<solver::Variable::shared_ptr, solver::Variable::shared_ptr> addRelationTypeConstraint(solver::Model::shared_ptr model,
                                                                                                    belem::Expression::shared_ptr realtion);
};

}



#endif // PARSER_H
