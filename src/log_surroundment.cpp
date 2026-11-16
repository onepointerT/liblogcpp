/**
 * @file logsurroundment.cpp
 * @brief Represent the before and the behind the message of a log record
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

#include "logsurroundment.hpp"


namespace logcpp {


LogSurroundment::LogSurroundment()
	:	before( *new LogFormat() )
	,	behind( *new LogFormat() )
	,	post( *new LogFormat() )
	,	srcloc( *new LogFormat() )
	,	env( *new env::LogEnvironment() )
{}

const std::string LogSurroundment::operator ^( const std::string msg ) {
	return this->post.formatAndReplace( this->env, msg );
}

} // namespace logcpp