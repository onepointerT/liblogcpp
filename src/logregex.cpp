/**
 * @file logregex.cpp
 * @brief Functions needed by `logcpp::regex::*`
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

#include "logregex.hpp"


namespace logcpp {
namespace regex {


Regex::Regex( const std::string rstr
	 		, const std::regex_constants::syntax_option_type syntx_options
	 		, const std::regex_constants::match_flag_type match_flag
)	:	rgx_str( rstr )
	,	rgx( rstr.c_str(), syntx_options )
	,	syntx_opts( syntx_options )
	,	match_flags( match_flag )
{}

Regex::MatchResultsType& Regex::searchIn( const std::string sstr ) const {
	std::cmatch& m = *new std::cmatch(); 
	std::regex_match( sstr.c_str(), m, this->rgx, this->match_flags );
	return m;
}

const std::string Regex::replaceIn( const std::string rstr, const char* fmt ) const {
	return std::regex_replace( rstr, this->rgx, fmt, this->match_flags );
}

} // namespace regex
} // namespace logcpp