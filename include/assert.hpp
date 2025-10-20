/**
 * @file assert.hpp
 * @brief This file enables assertion functions for LibLogC++
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

#include "config.hpp"


namespace logcpp {


/**
 * @brief Function that asserts something and logs one of two different messages depending on the assertion's result
 * @param condition The condition to be evaluated
 * @param fail Log action to perform on failure of the assertion ( condition != true )
 * @returns The result of the evaluated condition, either true or false
 */
template< typename logger_t >
bool assert( bool condition, logger_t& fail ) {
	bool assert_result = condition;
	if ( !assert_result ) {
		fail;
	}
	return assert_result;
}


/**
 * @brief Function that asserts something and logs one of two different messages depending on the assertion's result
 * @param condition The condition to be evaluated
 * @param success Log action to perform on success of the assertion ( condition == true )
 * @param fail Log action to perform on failure of the assertion ( condition != true )
 * @returns The result of the evaluated condition, either true or false
 */
template< typename logger_t >
bool assert( bool condition, logger_t& fail, logger_t& success ) {
	bool assert_result = condition;
	if ( assert_result ) {
		success;
	} else {
		fail;
	}
	return assert_result;
}

} // namespace logcpp

