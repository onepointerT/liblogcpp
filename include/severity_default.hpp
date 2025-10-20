/**
 * @file severity_default.hpp
 * @brief The default severities that are used in LibLogC++
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

#include "config.hpp"
#include "severity.hpp"


namespace logcpp {

/**
 * @brief The default severities that are used in LibLogC++
 * @note In all cases, the value 
 * @note - 0 defaults to off
 * @note - 1 defaults to critical
 * @note - The last element has to be called SEVERITY_SIZE to determine
 * @note   the size of the enum
 * @note
 * @note - You are not allowed to assign values to the enum members
 * @note
 * @note - For better use and readablity, a severity_level enum is defined at global
 * @note   library scope, but you also may define it in your own namespace.
 * @note
 * @note - The higher the enum value, the less critical is this severity
 */
enum default_severity_levels {
	off,
	critical,
	error,
	warning,
	normal,
	verbose,
	verbose2,
	debug,
	debug2,
	SEVERITY_SIZE
};


/**
 * @brief The default severity set that describes the enum default_severity_levels and is used in globallog and severity_logger
 */
class DefaultSeverity
	:	public AbstractSeverity< default_severity_levels >
{
public:
	/**
	 * @brief Constructor
	 */
	DefaultSeverity();

	/**
	 * @brief The names of severity_level as string array
	 */
	static std::array < const std::string, 1 + (int)default_severity_levels::SEVERITY_SIZE >* default_severity_names;
};

} // namespace logcpp

