/**
 * @file logenv.cpp
 * @brief Enables LibLogC++ to hold a set of values
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
	LibLogC++: An intuitive and highly customizable LGPL library for logging with C++.
	Copyright (C) 2015 Linux Gruppe IRB, TU Dortmund <linux@irb.cs.tu-dortmund.de>
	Copyright (C) 2015-2021,2026 Sebastian Lau <lauseb644@gmail.com>


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

#include <map>
#include <string>

namespace logcpp {

// Forward-declaration
class LogFormat;


namespace env {


class Value {
public:
    std::string name;
    std::string value;

    Value();
    Value( const std::string vname, const std::string vval );

    Value& operator=( const Value& other );
    static Value& operator()();

    bool empty() const;
};

class LogEnvironment
    :   protected std::map< std::string, Value >
{
public:
    LogEnvironment();

    Value& operator[]( const std::string key );
    Value& at( const std::string key ) const;

    const std::string format( const logcpp::LogFormat& formatter ) const;

    void clear();
};


} // namespace env
} // namespace logcpp