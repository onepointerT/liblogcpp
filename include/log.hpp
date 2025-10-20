/**
 * @file log.hpp
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

#include <memory>
#include <fstream>

#include "config.hpp"

#include "basic_log_input.hpp"
#include "severity_logger.hpp"

namespace logcpp {

/**
 * @brief A global singleton logger with severities that has a console and a file channel
 */
class globallog
	:  public severity_log< default_severity_levels >
{
private:
	static std::unique_ptr< globallog > log_;

	static std::string logfile;
	
	globallog();
	globallog(globallog const& another) = delete;
	globallog& operator=(globallog const& another);

protected:
    std::unique_ptr< severity_logger > console_log;
    std::unique_ptr< basic_log_input > console_input_log;

	bool file_log_enabled_;
	std::ofstream* ofs;
	std::unique_ptr< severity_logger > file_log;
	default_severity_levels file_severity;
    
	void enable_console_log_impl();
	void disable_console_log_impl();
	void enable_file_log_impl();
	void disable_file_log_impl();
	void set_logfile_impl();

public:

	/**
	 * @brief Get a reference to the logger object
	 */
	static globallog& get();

	/**
	 * @brief Override of severity_log::operator<< for functions
	 * @param f Some function that takes a reference to the globallog and returns it
	 */
	logcpp::globallog& operator<<(globallog& (*f)(globallog&));

	/**
	 * @brief Override of severity_log::operator<< for generic objects
	 * @param t Some object of type T that can be inserted into a std::ostream
	 */
	template< typename T >
	globallog& operator<<(const T& t) {
		this->log<T>(t);
		return *this;
	}
    
	/**
	 * @brief Forward declaration of basic_log_input::operator>> for functions
	 * @param f Some function that takes a reference to the globallog and returns it
	 */
	globallog& operator>>(globallog& (*f)(globallog& l));
    
	/**
	 * @brief Forward declaration of basic_log_input::operator>> for generic objects
	 * @note The function basic_log_input::log<T>(const T& t) has to be defined for this operator to work
	 * @param t Some object to log to this basic_log_input
	 */
    template< typename T >
	globallog& operator>>(const T& t) {
        *console_input_log >> t;
		return *this;
	}
	
	/**
     * @brief Forward declaration of basic_log_input::operator>> for input_flag objects
     * @param iflag The input flag to set for the next input
     */
    globallog& operator>>( const logcpp::input_flag iflag );


	/**
	 * @brief Set a file to log to and enable the file channel
	 * @param file A path to the file to log to
	 */
	static void set_logfile( std::string file );

	/**
	 * @brief Sets the maximum severity level of messages sent to the console log
	 * @param level The maximum severity level to be used
	 */
	void set_max_console_severity( default_severity_levels level );

	/**
	 * @brief Sets the maximum severity level of messages sent to log file
	 * @param level The maximum severity level to be used
	 */
	void set_max_file_severity( default_severity_levels level );

	/**
	 * @brief Sets the maximum severity level for both log channels (console and file)
	 * @param level The maximum severity level to be used
	 */
	void set_max_severity_level( default_severity_levels level);

	/**
	 * @brief Sets wether to use timestamps in console channel or not
	 * @param use Enable or disable timestamps
	 */
	void use_timestamps_console( bool use = true );

	/**
	 * @brief Sets wether to use timestamps on file channel or not
	 * @param use Enable or disable timestamps
	 */
	void use_timestamps_file( bool use = true );

	/**
	 * @brief Overrides basic_log::enable_timestamp for and applies their functionality to both channels (console and file)
	 */
	void enable_timestamp();
	/**
	 * @brief Overrides basic_log::disable_timestamp for and applies their functionality to both channels (console and file)
	 */
	void disable_timestamp();

	/**
	 * @brief Enable logging of severity names or not
	 * @param enable Enable or disable printing of severity names
	 */
	void enable_print_severity( bool enable = true );

	/**
	 * @brief Enables logging to console channel
	 */
	static void enable_console_log();

	/**
	 * @brief Disables logging to console channel
	 */
	static void disable_console_log();

	/**
	 * @brief Enables logging to a file. If no file was specified with set_logfile, all content will be logged to ./globallog
	 */
	static void enable_file_log();

	/**
	 * @brief Disables logging to a file
	 */
	static void disable_file_log();

	/**
	 * @return Wether logging to console channel is enabled or not
	 */
	bool console_log_enabled() const;
	/**
	 * @return Wether logging to file channel is enabled or not
	 */
	bool file_log_enabled() const;

	/**
	 * @brief Override of severity_log::end_record to insert end_record into both channels
	 */
	void end_record();
	/**
	 * @brief Override of severity_log::end_record to insert end_line into both channels
	 */
	void end_line();
    
    
	/**
     * @brief Forward declaration of basic_log_input::finalize
     */
    void finalize();
	
	/**
     * @brief Forward declaration of basic_log_input::input
     */
    void input();
    

	/**
	 * @brief Override of severity_log::log that inserts t into both channels (console and file)
	 * @param t Some object of type T that can be inserted into a std::ostream
	 */
	template< typename T >
	void log( const T& t) {
		*console_log << t;

		if( file_log_enabled_ ) {
			*file_log << t;
		}
	}

	/**
	 * @brief Override of severity_log::log that inserts the severity both channels (console and file)
	 * @param severity Insert this severity into stream and make it the current severity
	 */
	template< typename T >
	void log( const default_severity_levels& severity ) {
		if( this->stream.has_buffered_content() ) {
			this->log< std::string >( this->stream.get_buf() );
		}

		*console_log << severity;

		if( file_log_enabled_ ) {
			*file_log << severity;
		}

		this->current_severity = severity;
	}

	/**
	 * @brief Override of severity_log::log that inserts the severity both channels (console and file)
	 * @param sev_scope The severity_scope_t to log
	 */
	template<typename T>
	void log( const severity_scope_t& sev_scope ) {
		this->log< default_severity_levels >(sev_scope.first);
		this->log< scope_t >(sev_scope.second);
	}
    
    /**
     * @brief Forward declaration of basic_log_input::get_input
     * @param key The input flag key to look for
     * @returns The input for key or an empty string, if the key does not exist in the input collection
     */
    const basic_log_input::input_t get_input( const basic_log_input::input_flag_t key ) const;
    
    /**
     * @brief Forward declaration of basic_log_input::get_input_current
     * @returns The last input or an empty string, if there is no current input value
     */
    const basic_log_input::input_t get_input_current() const;
	
#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
	/**
	 * @brief Member function that controls colors and styles of the underlying sink
	 * @param mode Some value of color
	 */
	template< typename T >
	void log( const termmode& mode ) {
		if ( m_color_ok ) {
            *console_log << mode;
		}
	}
#endif
};


/**
 * @brief Template specialization for endl and globallog
 */
template globallog& endl(globallog&);

/**
 * @brief Template specialization for endrec and globallog
 */
template globallog& endrec(globallog&);

/**
 * @brief Template specialization for input and globallog
 */
template globallog& input(globallog&);

/**
 * @brief Template specialization for finalize and globallog
 */
template globallog& finalize(globallog&);


} // namespace logcpp

/**
 * @def stdlog
 * @brief Macro that can be used to insert log records into like 'stdlog << SOMECONTENT'
 */
#define stdlog logcpp::globallog::get()
