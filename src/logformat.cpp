/**
 * @file logformat.cpp
 * @brief Enable formatting of strings for the log library.
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

#include "logformat.hpp"


namespace logcpp {


LogFormat::LogFormat()
	:	logrgx( *new regex::Regex( "" ) )
	,	fmt("")
	,	rgx_active( false )
{}

LogFormat::LogFormat( const char* fmt_vars_str )
	:	logrgx( *new regex::Regex( "" ) )
	,	fmt( fmt_vars_str )
	,	rgx_active( false )
{}

LogFormat::LogFormat( const char* fmt_vars_str, const std::string lrgx )
	:	logrgx( *new regex::Regex( lrgx ) )
	,	fmt( fmt_vars_str )
	,	rgx_active( true )
{}



LogFormat& LogFormat::operator ^( const char* fmt_vars_str ) {
	this->fmt = fmt_vars_str;
	return *this;
}

LogFormat& LogFormat::operator ^( const LogFormat::RegexDefinitionT lrgx ) {
	this->logrgx = *new regex::Regex( lrgx.first, lrgx.second );
	return *this;
}


std::string LogFormat::formatAndReplace( const env::LogEnvironment& env
									   , const std::string logstream_or_fmtstr
) const {
	std::string fmt_replace = logstream_or_fmtstr;
	size_t pos = 0UL;
	while ( pos = fmt_replace.find_first_of( "${", pos ) != fmt_replace.npos ) {
		size_t pos_closing = fmt_replace.find_first_of( '}', pos );
		if ( pos_closing == fmt_replace.npos ) continue;

		std::string vname = fmt_replace.substr( pos+2, pos_closing - 1 - pos_closing + 2 );

		env::Value& val = env.at(vname);
		if ( ! val.empty() )
			fmt_replace = fmt_replace.replace( pos, pos_closing-pos, val.value );
	}

	return fmt_replace;
}

} // namespace logcpp