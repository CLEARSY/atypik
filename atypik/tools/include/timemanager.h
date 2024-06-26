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
#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <chrono>
#include <string>

namespace tools {

enum class TimeUnit {NanoSeconds, MicroSeconds, MilliSeconds, Seconds, Minutes, Hours};

class ChronoError : public std::exception
{
public:
    /*!
     * \brief Construct a ChronoError exception from a message
     * \param message
     * The message
     */
    ChronoError(std::string message);
    const char * what() const noexcept override;

private:
    std::string message_;
};
/*!
 * \brief The Chrono class is used to monitor the performances of atypik
 */
class Chrono {
public:
    /*!
     * \brief Instanciate a chrono
     */
    Chrono();
    /*!
     * \brief Start the chrono
     */
    void start();
    /*!
     * \brief Stop the chrono
     */
    void stop();
    /*!
     * \brief Reset the chrono
     */
    void reset();
    /*!
     * \brief Display the elapsed since the start of the chrono
     * \param time_unit
     * The time unit
     * \param message
     * The message to add before displaying the elapsed time
     */
    void displayElapsedTime(TimeUnit time_unit, std::string message="");

private:
    /*!
     * \brief The time when the chrono has started
     */
    std::chrono::high_resolution_clock::time_point start_;
    /*!
     * \brief The time when the chrono has stoped
     */
    std::chrono::high_resolution_clock::time_point stop_;
    /*!
     * \brief A boolean telling if the chrono has started
     */
    bool has_started;
};

}

#endif // TIMEMANAGER_H
