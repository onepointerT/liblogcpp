/**
 * @file logregex.hpp
 * @brief Functions and classes needed by regular expressions for logging and environment
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

#include <regex>

namespace logcpp {
namespace regex {

class Regex
{
public:
    const std::string_view rgx_str;
    const std::regex rgx;
    const std::regex_constants::syntax_option_type syntx_opts;
    const std::regex_constants::match_flag_type match_flags;

    Regex( const std::string rstr
         , const std::regex_constants::syntax_option_type syntx_options
                = std::regex_constants::syntax_option_type::_S_multiline
         , const std::regex_constants::match_flag_type match_flag
                = std::regex_constants::match_flag_type::_S_default
    );

    typedef typename std::cmatch MatchResultsType;
    typedef typename std::ssub_match MatchType;

    MatchResultsType& searchIn( const std::string sstr ) const;
    const std::string replaceIn( const std::string rstr, const char* fmt ) const;
};


} // namespace regex
} // namespace logcpp