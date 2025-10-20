/**
 * @file severity_logger.hpp
 * @brief A severity logger that can be used instantly
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
    LibLogC++: A intuitive and highly customizable LGPL library for logging with C++.
    Copyright (C) 2015 Linux Gruppe IRB, TU Dortmund <linux@irb.cs.tu-dortmund.de>
    Copyright (C) 2015-2021 Sebastian Lau <lauseb644@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/


#pragma once

#include <typeinfo>

#include "config.hpp"

#include "severity_log.hpp"
#include "severity_default.hpp"


namespace logcpp {

/**
 * @brief A severity logger that can be used instantly
 */
class severity_logger
    :	public severity_log< default_severity_levels >
{
protected:
    /**
     * @brief Function that optionally inserts colors for severities
     * @param severity Severity level severity level to log
     */
    virtual void log_severity( const default_severity_levels severity );

public:
    /**
     * @brief Override for severity_log::operator<<
     * @param f Some function that takes a reference to a severity_logger and returns it
     */
    severity_logger& operator<<(severity_logger& (*f)(severity_logger&)) {
        return f(*this);
    }

    /**
     * @brief Override for severity_log::operator<<
     * @param t Some object of type T that can be inserted into a std::ostream
     */
    template< typename T >
    severity_logger& operator<<(const T& t) {
        if ( new_record && enable_print_severity_
          && typeid(T) != typeid(default_severity_levels)
          && typeid(T) != typeid(severity_scope_t)
        ) {
            this->log_severity( this->severity() );
        }

        severity_log::log<T>(t);

        return *this;
    }

    /**
     * @brief Override for severity_log::operator<<
     * @param t Some object of type T that can be inserted into a std::ostream
     */
    template< typename T >
    severity_log& operator<<(const default_severity_levels severity) {
        this->log_severity( severity );
        return *this;
    }

    /**
     * @brief Creates severity_logger logging to std::cout
     * @param max_severity The maximum severity level for this logger
     */
    severity_logger( default_severity_levels max_severity = normal );

    virtual ~severity_logger();
    
    /**
     * @brief Creates severity_logger logging to a specific streambuf
     * @param stream A pointer to some std::streambuf where all content is logged to.
     * @param max_severity The maximum severity level for this logger
     */
    explicit severity_logger( std::streambuf* stream, default_severity_levels max_severity = normal );

    severity_logger( const severity_logger& ) = delete;
};


/**
 * @brief Template specialization for endl and default severities
 */
template severity_log< default_severity_levels >& endl(severity_log< default_severity_levels >&);
template severity_logger& endl(severity_logger&);

/**
 * @brief Template specialization for endrec and default severities
 */
template severity_log< default_severity_levels >& endrec(severity_log< default_severity_levels >&);
template severity_logger& endrec(severity_logger&);

} // namespace logcpp


/**
 * @def SCOPE_SEVERITY(lvl_)
 * @brief Insert a severity with the current scope into the severity_logger
 * @param lvl_ The severity level to be inserted into the severity_logger
 */
#define SCOPE_SEVERITY(lvl_) logcpp::severity_log< logcpp::default_severity_levels >::severity_scope(lvl_, SCOPE)


/**
 * @def CRITICAL
 * @brief Insert a critical scope into the log stream that may call the critical function of severity_log
 */
#define CRITICAL SCOPE_SEVERITY(logcpp::critical)

/**
 * @def ERROR
 * @brief Insert a error into the log stream
 */
#define ERROR SCOPE_SEVERITY(logcpp::error)

/**
 * @def WARNING
 * @brief Insert a warning into the log stream
 */
#define WARNING logcpp::warning

/**
 * @def VERBOSE
 * @brief Insert a verbose into the log stream
 */
#define VERBOSE logcpp::verbose

/**
 * @def VERBOSE2
 * @brief Insert a verbose2 into the log stream
 */
#define VERBOSE2 logcpp::verbose2

/**
 * @def DEBUG
 * @brief Insert a debug severity into the log stream
 */
#define DEBUG logcpp::debug

/**
 * @def DEBUG2
 * @brief Insert a debug2 scope into the log stream
 */
#define DEBUG2 SCOPE_SEVERITY(logcpp::debug2)
