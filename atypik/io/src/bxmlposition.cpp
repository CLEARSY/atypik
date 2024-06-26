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
#include "bxmlposition.h"

using namespace bxml;
using namespace tinyxml2;
using std::string;

Position::Position(int line, int column, int span, XMLElement *pElement)
    : line_(line), column_(column), span_(span), pElement_(pElement) {
  if (pElement->Attribute("tag")) tag_ = pElement->Attribute("tag");
}

int Position::getLine() { return line_; }

int Position::getColumn() { return column_; }

int Position::getSpan() { return span_; }

string Position::getTag() { return tag_; };

XMLElement *Position::getTinyXMLElement() { return pElement_; }
