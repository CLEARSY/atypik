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
#ifndef WRITER_H
#define WRITER_H

#include <map>
#include <memory>
#include <set>
#include <tinyxml2.h>

#include "expression.h"
#include "solverfactory.h"
#include "set.h"

namespace genericwriter
{
    class Writer
    {
    public:
        /*!
         * \brief A unique_ptr on a Writer
         */
        typedef std::unique_ptr<Writer> unique_ptr;
        /*!
         * \brief Write the same content as in the given document and add
         * rich typing
         * \param pDocument
         * The input document
         * \param output
         * The path of the file to create
         * \param expressions
         * The expressions
         * \param var_to_type
         * A map of shape variable -> type associating a type to each variable
         */
        void write(tinyxml2::XMLDocument *pDocument, std::string output,
                   std::unordered_set<belem::Expression::shared_ptr> expressions,
                   std::unordered_map<solver::Variable::shared_ptr, std::string> var_to_type);
        /*!
         * \brief Write on stdout the same content as in the given document and add
         * rich typing
         * \param pDocument
         * The input document
         * \param expressions
         * The expressions
         * \param var_to_type
         * A map of shape variable -> type associating a type to each variable
         */
        void write(tinyxml2::XMLDocument *pDocument,
                   std::unordered_set<belem::Expression::shared_ptr> expressions,
                   std::unordered_map<solver::Variable::shared_ptr, std::string> var_to_type);

    private:
        // Defining constants for tag
        const char *richtyperef_tag_ = "richtypref";
        const char *richtypesinfo_tag_ = "RichTypesInfo";
        const char *type_tag_ = "RichType";
        const char *id_tag_ = "id";
        const char *power_set_tag_ = "PowerSet";
        const char *product_tag_ = "CartesianProduct";
        const char *abstract_set_tag_ = "AbstractSet";
        const char *enumeration_tag_ = "EnumeratedSet";
        const char *enumerated_tag_ = "EnumeratedValue";
        const char *arg_tag_ = "arg";
        const char *arg1_tag_ = "arg1";
        const char *arg2_tag_ = "arg2";

        /*!
         * \brief A map associating an id to each type
         */
        std::unordered_map<std::string, int> types_;
        /*!
         * \brief A map associating an id to each set
         */
        std::unordered_map<std::string, int> sets_to_types_;
        /*!
         * \brief The document
         */
        tinyxml2::XMLDocument *pDocument_;
        /*!
         * \brief The expressions to write without the sets
         */
        std::unordered_set<belem::Expression::shared_ptr> expressions_;
        /*!
         * \brief The sets to write
         */
        std::vector<belem::Set::shared_ptr> sets_;
        /*!
         * \brief The default identifiers such as INTEGER, BOOL an FLOAT
         */
        std::unordered_set<std::string> default_identifiers_ = {"INTEGER", "BOOL", "REAL", "FLOAT", "STRING", "A"};
        /*!
         * \brief A map of shape variable -> type associating a type to each variable
         */
        std::unordered_map<solver::Variable::shared_ptr, std::string> var_to_type_;
        /*!
         * \brief Fill the types_ attribute
         */
        void computeTypes();
        /*!
         * \brief Add the rich type to each expression in the given map
         */
        void addRichTypes();
        /*!
         * \brief Add the Rich_Type tag
         */
        void addRichTypesInfo();
        /**
         * @brief Add POW and PRODUCT arguments if they are not already in the types_ map
         */
        void addArgsTypes();
        /*!
         * \brief Add the given type to a Type tag
         * \param pType
         * A pointer on the Type tag
         * \param type
         * The type to add
         */
        void addType(tinyxml2::XMLElement *pType, std::string type);
        /*!
         * \brief Add the given set type to a Type tag
         * \param pType
         * A pointer on the type tag
         * \param set
         * The set to add
         */
        void addSetType(tinyxml2::XMLElement *pType, belem::Set::shared_ptr set);
        /**
         * @brief Add the given power set type to a type tag
         * @param pType
         * A pointer on the type tag
         * @param set
         * The set to add
         */
        void addPowerSetType(tinyxml2::XMLElement *pType, belem::Set::shared_ptr set);
        /*!
         * \brief Compute the argument of a string representing a POW expression
         * \param pow
         * The POW expression
         * \return the argument of the POW expression
         */
        std::string getArgFromPow(const std::string &pow);
        /*!
         * \brief Compute the arguments A and B from q string representing a A x B
         * \param product
         * The product A x B
         * \return A and B
         */
        std::pair<std::string, std::string> getArgsFromCartesianProduct(const std::string &product);
    };
}

#endif // WRITER_H
