/**
 * @file log_exception.hpp
 * @brief A global singleton logger with severities that has a console and a file channel
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
	LibLogC++: An intuitive and highly customizable LGPL library for logging with C++.
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

#include <exception>


namespace logcpp {

/**
 * @brief A exception type that can be used with LibLogC++
 */
class critical_exception
	:	public std::exception
{
public:
	critical_exception()
		:	std::exception()
	{}

	/**
	 * @return Description of this exception
	 */
	const char* what() const noexcept {
		return "logcpp::critical_exception";
	}
};

/**
 * @brief A function that can be as critical function in severity_log. Only throws a critical_exception
 * @throws critical_exception
 */
void abort_with_exception() { throw critical_exception(); }

} // namespace logcpp

