/**
 * @file basic_log.hpp
 * @brief The underlying logging basics
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

#include "logstream.hpp"


#include <utility>
#include <iostream>
#include <sstream>

extern "C" {
#include <ctime>
}

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
#include "color_feature.hpp"
#endif




namespace logcpp {

/**
 * @brief A tuple containing a string and an unsigned int representing some place in a file
 */
typedef std::pair< std::string, unsigned int > scope_t;
/**
 * @return Free function that generates a scope_t from its parameters
 * @param place The string of the place wanted for this scope
 * @param line An unsigned integer representing the line for this scope
 */
inline scope_t scope(std::string place, unsigned int line) {
#ifndef LOGCPP_LEAVE_SCOPE_DIRS_PREFIX
	unsigned int dir_prefix_end = 0;
#ifdef __unix__
	dir_prefix_end = place.find_last_of('/');
#elif _WIN32
	dir_prefix_end = place.find_last_of('\\');
#endif
	if ( dir_prefix_end != std::string::npos && dir_prefix_end != 0 ) {
		place = place.substr(dir_prefix_end+1);
	}
#endif
	return scope_t( place, line);
}

/**
 * @def SCOPE
 * @brief The scope_t of the file where the macro is used. Equivalent to scope
 */
#define SCOPE logcpp::scope(__FILE__, __LINE__)

/**
 * @returns A string with the current time like %DATE_%TIME
 */
inline const std::string timestr() {
	std::time_t rawtime = std::time(0);
	struct std::tm* tinfo;
	tinfo = std::localtime(&rawtime);
	char buf[22];
	std::strftime(buf, 22, "%F_%T", tinfo);
	return std::string(buf);
}

/**
 * @def TIME
 * @brief A macro equivalent to timestr
 */
#define TIME logcpp::timestr()

/**
 * @brief The basic logging object
 */
class basic_log
{
public:

	/**
	 * @brief Insert some function into this basic_log
	 * @param f A function getting a basic_log and returning a basic_log
	 */
	basic_log& operator<<(basic_log& (*f)(basic_log& l)) {
		return f(*this);
	}

	/**
	 * @brief Insert some object into this basic_log
	 * @note The function basic_log::log<T>(const T& t) has to be defined for this operator to work
	 * @param t Some object to log to this basic_log
	 */
	template< typename T >
	basic_log& operator<<(const T& t) {
		log<T>(t);
		return *this;
	}


protected:
	/**
	 * @brief The underlying stream buffer for this basic_log
	 */
	logstreambuf stream;
	/**
	 * @brief Flag, if inserting the timestamp into log is enabled or not
	 */
	bool timestamp_enabled_;
	/**
	 * @brief Inserts the timestamp into log, if #timestamp_enabled_ is set to true
	 */
	void insert_time_or_not() {
		if(timestamp_enabled_) {
			stream << "[" << timestr() << "] - ";
		}
	}

	/**
	 * @brief Flag, if this record just began or contains content already
	 */
	bool new_record;

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
	bool m_color_ok;
	color_feature* m_color;
#endif

public:
    
    /**
     * @brief Check, if the current record is newly created
     */
    bool is_new_record() const {
        return new_record;
    }

	/**
	 * @brief Contructor. Specify, where to log to.
	 * @param outbuf A pointer to some std::streambuf where all content is logged to. Defaults to std::cout.rdbuf()
	 */
	explicit basic_log( std::streambuf* outbuf = std::cout.rdbuf() )
	    :	stream( outbuf )
	    ,	timestamp_enabled_(false)
	    ,	new_record(true)
#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
	    ,	m_color_ok(false)
	    ,	m_color( new color_feature() )

	{
		m_color_ok = stream.sink_is_terminal();
	}
#else
	{}
#endif
	basic_log( const basic_log& ) = delete;
	~basic_log() {}

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
	/**
	 * @brief Member function that controls colors and styles of the underlying sink
	 * @param mode Some value of color
	 */
	template< typename T >
	void log( const termmode& mode ) {
		if ( m_color_ok ) {
			stream << m_color->code(mode);
		}
	}
#endif

    template< typename T >
    void log( const std::string str ) {
        stream << str.c_str();
    }
	
	/**
     * @brief Flush the current buffer to output
     */
    void flush() {
        stream.flush();
        new_record = true;
    }

	/**
	 * @brief Member function that inserts a newline into the buffer, flushes it and begins a new record
	 */
	void end_record() {
#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
	if ( m_color_ok ) {
		this->log<termmode>(logcpp::ctl_reset_all);
	}
#endif
	    stream << "\n";
        
        this->flush();
	}

	/**
	 * @brief Enable logging the timestamp of a log record
	 */
	void enable_timestamp() { timestamp_enabled_ = true; }
	/**
	 * @brief Disable logging the timestamp of a log record
	 */
	void disable_timestamp() { timestamp_enabled_ = false; }

	/**
	 * @brief Member function that inserts a newline into buffer without flushing it
	 */
	void end_line() {
		stream << "\n";
		new_record = false;
	}

	/**
	 * @brief Member function that logs objects of the type scope_t
	 * @param scope The scope_t object to log
	 */
	template< typename T >
	void log( const scope_t& scope) {
		if( new_record ) insert_time_or_not();
		stream << "[ " << scope.first << ":" << scope.second << " ] : ";
		new_record = false;
	}

	/**
	 * @brief Member function that logs generic objects that have no special function
	 * @param t Some object of type T that can be inserted into a std::ostream
	 */
	template< typename T >
	void log(const T& t) {
		if( new_record ) insert_time_or_not();
		stream << t;
		new_record = false;
	}
};

/**
 * @brief Free function that ends the current record of a logger
 * @param out A logger object providing the function end_record()
 */
template< typename logger_t >
inline logger_t& endrec(logger_t& out) {
	out.end_record();
	return out;
}

/**
 * @brief Free function that ends a line in the buffer of a logger
 * @param out A logger object providing the function end_line()
 */
template< typename logger_t >
inline logger_t& endl(logger_t& out) {
	out.end_line();
	return out;
}

/**
 * @brief Template specialization for endl and basic_log
 */
template basic_log& endl(basic_log&);

/**
 * @brief A simple logger
 */
typedef basic_log logger;

} // namespace logcpp


/**
 * @def ENDL
 * @brief Insert a endrec into the log stream
 */
#define ENDL logcpp::endl


/**
 * @def ENDREC
 * @brief Insert a endrec into the log stream
 */
#define ENDREC logcpp::endrec
