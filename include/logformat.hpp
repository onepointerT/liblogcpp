/**
 * @file logformat.hpp
 * @brief Enable formatting of strings for the log library.
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

#include "logenv.hpp"
#include "logregex.hpp"
#include "source_location.hpp"

#include <string>
#include <utility>


namespace logcpp {

// Forward-declaration
namespace env { class LogEnvironment; class Value; } // namespace env

/**
 * @brief If some log message or a record's prefix is inserted into the log,
 * 		it can be quickly formatted with this class.
 * 
 */
class LogFormat {
public:
	/** @brief The regex that thus determines what to log from a string inserted. */
	regex::Regex& logrgx;
	/** 
	 * @brief The formatter that thus is to be followed.
	 * 		With use of a `LogEnvironment` this can be variables like `${filename}`,
	 * 		whereby $#s inserts the string or logstream and $#sr inserts the regex-filtered
	 * 		string or logstream. Further formats:
	 * 	* `$#f` -- The filename
	 *  * `$#l` -- The line number
	 *  * `$#fn` -- The function name
	 *  * `$#c` -- The column number
	 */
	std::string fmt;
	/**
	 * @brief 
	 * 
	 */
	bool rgx_active;

	LogFormat();
	LogFormat( const char* fmt_vars_str );
	LogFormat( const char* fmt_vars_str, const std::string lrgx = "*" );

	typedef typename std::pair< std::string
							  , std::regex_constants::syntax_option_type
				> RegexDefinitionT;
	
	template< class logger_t >
	using LogInput = std::pair< std::string, std::shared_ptr<logger_t> >;


	LogFormat& operator ^( const char* fmt_vars_str );
	LogFormat& operator ^( const RegexDefinitionT lrgx );

	std::string formatAndReplace( const env::LogEnvironment& env
								, const std::string logstream_or_fmtstr
	) const;

	template< class logger_t >
	const std::string& operator |( const LogInput<logger_t> input ) const {
		std::string s_fmt = this->formatAndReplace(input.second->env, this->fmt);
		
		std::string s = s_fmt;
		size_t pos = 0UL;
		while ( s.find_first_of("$#s") != s.npos ) {
			pos = s.find_first_of( "$#sr", pos );
			if ( pos != s.npos ) {
				typename regex::Regex::MatchResultsType& m = this->logrgx.searchIn(input.first);
				if ( m.length() > 0 )
					s = s.replace( pos, 4, m[0].str() );
			} else {
				pos = s.find_first_of( "$#s", pos );
				if ( pos != s.npos )
					s = s.replace( pos, 3, input.first );
			}
		}

		pos = 0UL;
		while ( pos = s.find_first_of( "$#f", pos ) != s.npos ) { s = s.replace( pos, 3, input.second->env["file"].value ); }
		pos = 0UL;
		while ( pos = s.find_first_of( "$#c", pos ) != s.npos ) { s = s.replace( pos, 3, input.second->env["column"].value ); }
		pos = 0UL;
		while ( pos = s.find_first_of( "$#fn", pos ) != s.npos ) { s = s.replace( pos, 4, input.second->env["func"].value ); }
		pos = 0UL;
		while ( pos = s.find_first_of( "$#l", pos ) != s.npos ) { s = s.replace( pos, 3, input.second->env["line"].value ); }

		return *new std::string( s );
	}
};


} // namespace logcpp

