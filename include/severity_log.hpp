/**
 * @file severity_log.hpp
 * @brief Abstract description of a severity logger
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

#include "basic_log.hpp"
#include "severity.hpp"
#include "severity_feature.hpp"
#include "logmanip.hpp"


namespace logcpp {

/**
 * @brief Abstract description of a severity logger
 * @note severity_t: The type of severity that is to be used
 * @note severity_name: A function taking a severity_t and returning its name
 * @note max_name_length: A function returning the amount of characters of the longest severity name (used for alignment)
 */
template< typename severity_t >
class severity_log
    :	public basic_log,
        public severity_feature< severity_t >
{
protected:
	/**
	 * @brief Function that sets the severity
	 * @param severity Severity level severity level to log
	 */
	virtual void log_severity( const severity_t severity ) {
		this->current_severity = severity;
		if( enable_print_severity_ ) {
			stream << std::setw(m_severity->max_name_length() - m_severity->severity_name( this->current_severity ).length() ) << std::setfill(' ') << "<" << m_severity->severity_name( this->current_severity ) << ">: ";
		}
		this->new_record = false;
	}

	bool enable_print_severity_;
	AbstractSeverity< severity_t >* m_severity;

	/**
	 * @brief Optional function to be called on critical severity
	 */
	void(*abort_f)(void);
    
public:
	/**
	 * @brief Override for basic_log::operator<<
	 * @param f Some function that takes a reference to a severity_log and returns it
	 */
	severity_log& operator<<(severity_log& (*f)(severity_log&)) {
		return f(*this);
	}

	/**
	 * @brief Override for basic_log::operator<<
	 * @param t Some object of type T that can be inserted into a std::ostream
	 */
	template< typename T >
	severity_log& operator<<(const T& t) {
		log<T>(t);
		return *this;
	}

	/**
	 * @brief Constructor
	 * @param max_severity The maximum severity level for this logger
	 * @param outbuf A pointer to some std::streambuf where all content is logged to. Defaults to std::cout.rdbuf()
	 */
	explicit severity_log( AbstractSeverity< severity_t >* severity
	                     , severity_t max_severity
	                     , std::streambuf* outbuf = std::cout.rdbuf()
	)
	    :	basic_log(outbuf)
	    ,	severity_feature< severity_t >(max_severity)
	    ,	enable_print_severity_(true)
	    ,	m_severity( severity )
	    ,	abort_f(nullptr)
	{}
	severity_log( const severity_log& ) = delete;
    
    virtual ~severity_log() {}

	/**
	 * @brief Override of basic_log::end_record that only logs when it is enabled by severity
	 */
	void end_record() {
		if( this->log_enabled() ) {
			basic_log::end_record();
			if( this->current_severity == 1 && abort_f != nullptr ) {
				abort_f();
			}
		} else {
			stream.clear_buf();
			new_record = true;
		}
	}

	/**
	 * @brief Sets the function to be called when a critical record has been written (defaults to no function)
	 * @param crit_f Function to be called after log records with severity level 1
	 */
	void set_critical_log_function( void(*crit_f)(void) = nullptr ) {
		abort_f = crit_f;
	}

	/**
	 * @brief Override of basic_log::log that inserts the severity into stream first, if enabled
	 * @param t Some object of type T that can be inserted into a std::ostream
	 */
	template< typename T >
	void log( const T& t) {
		if( new_record ) {
			insert_time_or_not();
			if( enable_print_severity_ ) {
				stream << std::setw(m_severity->max_name_length() - m_severity->severity_name( this->current_severity ).length() ) << std::setfill(' ') << "<" << m_severity->severity_name( this->current_severity ) << ">: " << std::setfill(' ');
			}
			new_record = false;
		}
		basic_log::log<T>(t);
	}

	/**
	 * @brief Function that logs a severity inserted into this log stream and sets the current severity to its value
	 * @param severity Insert this severity into stream and make it the current severity
	 */
	template< typename T >
	void log( const severity_t& severity) {
		if( !new_record && stream.has_buffered_content() ) {
			this->end_record();	// Flush buffer with previous severity before changing the current
		} else if( new_record ) {
			insert_time_or_not();
		}
		this->log_severity( severity );
	}

	/**
	 * @brief A tuple containing a severity and a scope_t
	 */
	typedef std::pair< severity_t, scope_t > severity_scope_t;
	/**
	 * @brief Free function that generates a severity_scope_t from its parameters
	 */
	static severity_scope_t severity_scope( severity_t severity, scope_t scope ) {
		return severity_scope_t( severity, scope );
	}
	/**
	 * @brief Log a severity_scope_t to this log stream
	 * @param sev_scope The severity_scope_t to log
	 */
	template< typename T >
	void log( const severity_scope_t& sev_scope ) {
		this->log< severity_t >(sev_scope.first);
		this->log< scope_t >(sev_scope.second);
	}

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
	/**
	 * @brief Member function that can send colors before start of
	 * @param mode Some value of color
	 */
	template< typename T >
	void log( const termmode& mode ) {
		if ( m_color_ok ) {
			basic_log::log< termmode >( mode );
		}
	}
#endif

	/**
	 * @brief Enable or disable the logging of severity names into the log stream
	 * @param enable Enable or disable the loggin of severity names
	 */
	void enable_print_severity( bool enable = true ) { enable_print_severity_ = enable; }
};



} // namespace logcpp



