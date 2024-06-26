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
#ifndef CSVREADER_H
#define CSVREADER_H

#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace test {

class CSVRow {
public:
    /*!
     * \brief Set a delimiter for a new csv row
     * \param delimiter
     * The delimiter used to parse
     */
    void setDelimiter(std::string delimiter);
    /*!
     * \brief Accessor on an element of a row
     * \param index
     * The column to access
     * \return the element in the row corresponding to the given column
     */
    std::string operator[](std::size_t index) const;
    /*!
     * \brief An accessor on the size of row
     * \return the size of the row
     */
    std::size_t size() const;
    /*!
     * \brief Read the next row of a stream given a delimiter
     * \param stream
     * The stream to read
     */
    void readNextRow(std::istream& stream);

private:
    /*!
     * \brief The parsed line
     */
    std::string line_;
    /*!
     * \brief The indexes of each column
     */
    std::vector<int> data_;
    /*!
      * \brief The delimiter used to parse the line
      */
    std::string delimiter_;

};

class CSVIterator {
public:
    /*!
     * \brief Create a CSV iterator from a stream
     * \param stream
     * The stream to iterate on
     * \param delimiter
     * The delimiter used to parse
     */
    CSVIterator(std::istream& stream, std::string delimiter);
    /*!
     * \brief Create an empty CSV iterator
     * \param delimiter
     * The delimiter used to parse
     */
    CSVIterator(std::string delimiter);
    CSVIterator& operator++();
    CSVIterator operator++(int);
    bool operator==(CSVIterator const& rhs) const;
    bool operator!=(CSVIterator const& rhs) const;
    /*!
     * \brief An accessor on a reference of the current row
     * \return a reference on the current row
     */
    CSVRow const& operator*() const;
    /*!
     * \brief An accessor on a pointer of the current row
     * \return a pointer on the current row
     */
    CSVRow const* operator->() const;

private:
    /*!
     * \brief The stream to iterate on
     */
    std::istream* stream_;
    /*!
     * \brief The current row
     */
    CSVRow row_;
    /*!
     * \brief The delimiter used to parse
     */
    std::string delimiter_;
};

class CSVRange{
public:
    /*!
     * \brief Construct a csv range from a stream
     * \param stream
     * The stream to iterate
     * \param delimiter
     * The delimiter used to parse the csv
     * \param skip_header
     * If sets, skip the first row
     */
    CSVRange(std::ifstream& stream, std::string demlimiter, bool skip_header=false);
    /*!
     * \brief An accessor on the first element of the range
     * \return the first element of the range
     */
    CSVIterator begin() const;
    /*!
     * \brief An accessor on the last element of the range
     * \return the last element of the range
     */
    CSVIterator end() const;
private:
    /*!
     * \brief The stream to iterate on
     */
    std::ifstream& stream_;
    /*!
     * \brief The delimiter used to parse the file
     */
    std::string delimiter_;
    /*!
     * \brief Tells if the first row has to be skiped
     */
    bool skip_header_;
};

}

#endif // CSVREADER_H
