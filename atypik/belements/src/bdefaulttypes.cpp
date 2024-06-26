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
#include "bdefaulttypes.h"

using namespace belem;
using std::string;

DefaultBType::DefaultBType(std::string value,
                           bxml::Position::shared_ptr position):
    Expression({position}), value_(value) {}

BBool::BBool(std::string value,
             bxml::Position::shared_ptr position):
    DefaultBType(value, position) {}

BInteger::BInteger(std::string value,
                   bxml::Position::shared_ptr position):
    DefaultBType(value, position) {}

BReal::BReal(std::string value,
             bxml::Position::shared_ptr position):
    DefaultBType(value, position) {}

BString::BString(std::string value,
                 bxml::Position::shared_ptr position):
    DefaultBType(value, position) {}
