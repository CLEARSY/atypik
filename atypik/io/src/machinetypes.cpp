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
#include "machinetypes.h"

namespace genericparser {
UnknownMachineType::UnknownMachineType(std::string type)
    : message_("Machine type " + type + " is unknown") {}

const char* UnknownMachineType::what() const noexcept {
  return message_.c_str();
}

MachineType computeType(std::string type) {
    if (type == "abstraction")
        return Abstraction;
    if (type == "refinement")
        return Refinement;
    if (type == "implementation")
        return Implementation;
    throw UnknownMachineType(type);
}

}
