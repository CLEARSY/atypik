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
#include "csvreader.h"

#include "csverror.h"

using std::ifstream;
using std::istream;
using std::size_t;
using std::string;
using std::to_string;
using std::vector;

namespace test {

void CSVRow::setDelimiter(string delimiter) { delimiter_ = delimiter; }

string CSVRow::operator[](size_t index) const {
  if (size() <= index)
    throw CSVError("Index " + std::to_string(index) +
                   " out of range for row of length " + std::to_string(size()) +
                   ".");

  return string(&line_[data_[index] + 1],
                data_[index + 1] - (data_[index] + 1));
}

size_t CSVRow::size() const { return data_.size() - 1; }

void CSVRow::readNextRow(istream& stream) {
  std::getline(stream, line_);
  data_.clear();
  data_.emplace_back(-1);
  string::size_type pos = 0;
  while ((pos = line_.find(delimiter_, pos)) != string::npos) {
    data_.emplace_back(pos);
    pos++;
  }
  pos = line_.size();
  data_.emplace_back(pos);
}

CSVIterator::CSVIterator(istream& stream, string delimiter)
    : stream_(stream.good() ? &stream : nullptr), delimiter_(delimiter) {
  row_.setDelimiter(delimiter);
  (*this)++;
}

CSVIterator::CSVIterator(string delimiter)
    : stream_(nullptr), delimiter_(delimiter) {
  row_.setDelimiter(delimiter);
}

istream& operator>>(istream& stream, CSVRow& data) {
  data.readNextRow(stream);
  return stream;
}

CSVIterator& CSVIterator::operator++() {
  if (stream_) {
    if (not((*stream_) >> row_)) {
      stream_ = nullptr;
    }
  }
  return *this;
}

CSVIterator CSVIterator::operator++(int) {
  CSVIterator result(*this);
  ++(*this);
  return result;
}

bool CSVIterator::operator==(const CSVIterator& rhs) const {
  return ((&rhs == this) or
          ((this->stream_ == nullptr) and (rhs.stream_ == nullptr)));
}

bool CSVIterator::operator!=(const CSVIterator& rhs) const {
  return not((*this) == rhs);
}

CSVRow const& CSVIterator::operator*() const { return row_; }

CSVRow const* CSVIterator::operator->() const { return &row_; }

CSVRange::CSVRange(ifstream& stream, string delimiter, bool skip_header)
    : stream_(stream), delimiter_(delimiter), skip_header_(skip_header) {}

CSVIterator CSVRange::begin() const {
  CSVIterator result = CSVIterator(stream_, delimiter_);
  if (skip_header_ and result != end()) return ++result;
  return result;
}

CSVIterator CSVRange::end() const { return CSVIterator(delimiter_); }

}  // namespace test
