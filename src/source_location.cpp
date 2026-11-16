/**
 * @file source_location.cpp
 * @brief Get information from where the source code is.
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

#include "source_location.hpp"


namespace logcpp {

SourceLocation::SourceLocation( const std::source_location srcl )
	:	src_loc( srcl )
{}


void SourceLocation::get( env::LogEnvironment& env ) const {
	env["file"]() = env::Value( "file", this->src_loc.file_name() );
	env["func"]() = env::Value( "func", this->src_loc.function_name() );
	env["line"]() = env::Value( "line", std::to_string(this->src_loc.line()) );
	env["column"]() = env::Value( "column", std::to_string(this->src_loc.column()) );
}
    
/*SourceLocation SourceLocation::operator=( std::source_location srcl ) {
	this->src_loc = srcl;
	return *this;
}*/

} // namespace logcpp