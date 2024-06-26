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
#include "writer.h"

using namespace tinyxml2;

using belem::Expression;
using belem::Set;
using solver::Variable;
using std::dynamic_pointer_cast;
using std::pair;
using std::set;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

namespace genericwriter
{
  void Writer::write(XMLDocument *pDocument, string output,
                     unordered_set<Expression::shared_ptr> expressions,
                     unordered_map<Variable::shared_ptr, string> var_to_type)
  {
    for (auto &expression : expressions)
    {
      Set::shared_ptr set_cast = dynamic_pointer_cast<Set>(expression);
      if (set_cast != nullptr)
        sets_.push_back(set_cast);
      else
        expressions_.insert(expression);
    }
    var_to_type_ = var_to_type;
    pDocument_ = pDocument;
    computeTypes();
    addRichTypes();
    addRichTypesInfo();
    pDocument_->SaveFile(output.c_str());
  }

  void Writer::write(XMLDocument *pDocument,
                     unordered_set<Expression::shared_ptr> expressions,
                     unordered_map<Variable::shared_ptr, string> var_to_type)
  {
    for (auto &expression : expressions)
    {
      Set::shared_ptr set_cast = dynamic_pointer_cast<Set>(expression);
      if (set_cast != nullptr)
        sets_.push_back(set_cast);
      else
        expressions_.insert(expression);
    }
    var_to_type_ = var_to_type;
    pDocument_ = pDocument;
    computeTypes();
    addRichTypes();
    addRichTypesInfo();
    pDocument_->Print();
  }

  void Writer::computeTypes()
  {
    int id = sets_.size();
    for (auto &[var, type] : var_to_type_)
    {
      types_[type] = id++;
    }

    id = 0;
    unordered_set<string> added_sets;

    for (auto &set : sets_)
    {
      if (added_sets.contains(set->getID()->format()))
        continue;
      added_sets.insert(set->getID()->format());
      // The type id
      sets_to_types_[set->getID()->format()] = id++;
      // The type POW(id)
      sets_to_types_[var_to_type_[set->getAssociatedVariable()]] = id++;
    }
    addArgsTypes();
  }

  void Writer::addRichTypes()
  {
    for (auto expression : expressions_)
    {
      string type = var_to_type_[expression->getAssociatedVariable()];
      for (auto position : expression->getPositions())
      {
        XMLElement *pExpr = position->getTinyXMLElement();
        int id = types_[var_to_type_[expression->getAssociatedVariable()]];
        pExpr->SetAttribute(richtyperef_tag_, id);
      }
    }
  }

  void Writer::addArgsTypes()
  {
    vector<string> stack;

    auto addArg = [this, &stack](string &arg)
    {
      if (not types_.contains(arg))
      {
        stack.push_back(arg);
        types_[arg] = types_.size();
      }
    };

    for (const auto &[type, _] : types_)
      if (not sets_to_types_.contains(type))
        stack.push_back(type);

    types_ = sets_to_types_;

    while (not stack.empty())
    {
      string type = stack.back();
      stack.pop_back();

      while (type.starts_with(' '))
        type.erase(0, 1);

      while (type.back() == ' ')
        type.pop_back();

      if (type.starts_with("(POW"))
      {
        string arg = getArgFromPow(type);
        addArg(arg);
      }
      else if (type.starts_with("(PRODUCT"))
      {
        pair<string, string> args = getArgsFromCartesianProduct(type);
        addArg(args.first);
        addArg(args.second);
      }

      addArg(type);
    }
  }

  void Writer::addRichTypesInfo()
  {
    vector<string> types(types_.size());
    int sets_index = 0;

    for (auto &[type, id] : types_)
    {
      if (sets_to_types_.contains(type))
      {
        ++sets_index;
        continue;
      }
      types[id] = type;
    }

    XMLElement *pTypesInfo = pDocument_->NewElement(richtypesinfo_tag_);

    int index = 0;
    unordered_set<string> added_sets;
    for (auto set : sets_)
    {
      if (added_sets.contains(set->getID()->format()))
        continue;
      added_sets.insert(set->getID()->format());
      XMLElement *pType = pDocument_->NewElement(type_tag_);
      pType->SetAttribute(id_tag_, index++);
      addSetType(pType, set);
      pTypesInfo->LinkEndChild(pType);
      pType = pDocument_->NewElement(type_tag_);
      pType->SetAttribute(id_tag_, index++);
      addPowerSetType(pType, set);
      pTypesInfo->LinkEndChild(pType);
    }

    for (unsigned int id = sets_index; id < types.size(); id++)
    {
      XMLElement *pType = pDocument_->NewElement(type_tag_);
      pType->SetAttribute(id_tag_, id);
      addType(pType, types[id]);
      pTypesInfo->LinkEndChild(pType);
    }

    pDocument_->FirstChildElement()->LinkEndChild(pTypesInfo);
  }

  void Writer::addType(XMLElement *pType, string type)
  {
    if (type.starts_with("(POW") and not sets_to_types_.contains(type))
    {
      XMLElement *pPow = pDocument_->NewElement(power_set_tag_);
      pPow->SetAttribute(arg_tag_, types_[getArgFromPow(type)]);
      pType->LinkEndChild(pPow);
    }
    else if (type.starts_with("(PRODUCT"))
    {
      XMLElement *pCartesianProduct = pDocument_->NewElement(product_tag_);
      pair<string, string> args = getArgsFromCartesianProduct(type);
      pCartesianProduct->SetAttribute(arg1_tag_, types_[args.first]);
      pCartesianProduct->SetAttribute(arg2_tag_, types_[args.second]);
      pType->LinkEndChild(pCartesianProduct);
    }
    else
    {
      if (type[0] == '(' and type.back() == ')')
      {
        type.erase(0, 1);
        type.pop_back();
      }
      // If it is an default identifier, we create a type
      if (default_identifiers_.contains(type))
      {
        XMLElement *pNewType = pDocument_->NewElement(type.c_str());
        pType->LinkEndChild(pNewType);
      }
      // Else, it is a set and we look for (POW id)
      else
      {
        XMLElement *pNewType = pDocument_->NewElement(power_set_tag_);
        pNewType->SetAttribute(arg_tag_, types_[type]);
        pType->LinkEndChild(pNewType);
      }
    }
  }

  void Writer::addSetType(tinyxml2::XMLElement *pType, Set::shared_ptr set)
  {
    if (set->isAnEnumeration())
    {
      XMLElement *pEnumeration = pDocument_->NewElement(enumeration_tag_);
      pEnumeration->SetAttribute(id_tag_, set->getID()->format().c_str());
      for (auto &value : set->getEnumeratedValues())
      {
        XMLElement *pEnumerated = pDocument_->NewElement(enumerated_tag_);
        pEnumerated->SetAttribute(id_tag_, value->format().c_str());
        pEnumeration->LinkEndChild(pEnumerated);
      }
      pType->LinkEndChild(pEnumeration);
    }
    else
    {
      XMLElement *pAbstractSet = pDocument_->NewElement(abstract_set_tag_);
      pAbstractSet->SetAttribute(id_tag_, set->getID()->format().c_str());
      pType->LinkEndChild(pAbstractSet);
    }
  }

  void Writer::addPowerSetType(tinyxml2::XMLElement *pType, Set::shared_ptr set)
  {
    string str_set = set->getID()->format();
    XMLElement *pPow = pDocument_->NewElement(power_set_tag_);
    pPow->SetAttribute(arg_tag_, types_[str_set]);
    pType->LinkEndChild(pPow);
  }

  string Writer::getArgFromPow(const string &pow)
  {
    string copy = pow;
    copy.erase(0, 5);
    copy.pop_back();
    return copy;
  }

  pair<string, string> Writer::getArgsFromCartesianProduct(const string &product)
  {
    string copy = product;
    copy.erase(0, 9);
    copy.pop_back();
    string arg1, arg2;
    int opening_parenthesis = 0;
    int arg2_pos = 0;
    for (auto character : copy)
    {
      arg2_pos++;
      if (character == '(')
        opening_parenthesis++;
      if (character == ')')
        opening_parenthesis--;
      if (opening_parenthesis == 0)
        break;
      arg1 += character;
    }
    if (arg1 == "")
    {
      arg2_pos = copy.find_first_of(' ');
    }
    arg1 = copy.substr(0, arg2_pos);
    // The +1 is to remove the space between the args
    arg2 = copy.substr(arg2_pos + 1, copy.size());
    return {arg1, arg2};
  }

} // namespace genericwriter
