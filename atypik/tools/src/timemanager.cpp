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
#include "timemanager.h"

#include <iostream>

using namespace std::chrono;
using std::cout;
using std::endl;
using std::string;

namespace tools {

// Implementation of the ChronoError Exception

ChronoError::ChronoError(string message) : message_(message) {}

const char* ChronoError::what() const noexcept { return message_.c_str(); }

// Implementation of the chrono class

Chrono::Chrono() { reset(); }

void Chrono::start() {
  if (has_started) throw ChronoError("Chrono has already started");
  has_started = true;
  start_ = high_resolution_clock::now();
}

void Chrono::stop() {
  if (not has_started) throw ChronoError("Chrono has not started");
  has_started = false;
  stop_ = high_resolution_clock::now();
}

void Chrono::reset() {
  has_started = false;
  start_ = high_resolution_clock::now();
}

void Chrono::displayElapsedTime(TimeUnit time_unit, string message) {
  switch (time_unit) {
    case TimeUnit::NanoSeconds: {
      std::chrono::duration<double, std::nano> nano =
          high_resolution_clock::now() - start_;
      cout << message << duration_cast<nanoseconds>(nano).count() << " ns"
           << endl;
      break;
    }
    case TimeUnit::MicroSeconds: {
      std::chrono::duration<double, std::milli> micro =
          high_resolution_clock::now() - start_;
      cout << message << micro.count() << " µs" << endl;
      break;
    }
    case TimeUnit::MilliSeconds: {
      std::chrono::duration<double, std::milli> milli =
          high_resolution_clock::now() - start_;
      cout << message << milli.count() << " ms" << endl;
      break;
    }
    case TimeUnit::Seconds: {
      std::chrono::duration<double> seconds =
          high_resolution_clock::now() - start_;
      cout << message << seconds.count() << " s" << endl;
      break;
    }
    case TimeUnit::Minutes: {
      cout << message
           << duration_cast<std::chrono::minutes>(high_resolution_clock::now() -
                                                  start_)
                  .count()
           << " min" << endl;
      break;
    }
    case TimeUnit::Hours: {
      cout
          << message
          << duration_cast<hours>(high_resolution_clock::now() - start_).count()
          << " h" << endl;
    }
  }
}

}  // namespace tools
