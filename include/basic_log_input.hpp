/**
 * @file basic_log_input.hpp
 * @brief The underlying logging basics for log input by command line
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

#include "basic_log.hpp"


#include <iostream>
#include <map>
#include <utility>
#include <vector>




namespace logcpp {


class input_flag {
protected:
    std::string in_flag;
    
public:
    input_flag( const std::string iflag );
    
    const std::string operator()() const;
};


class input_query
    :   public std::vector< std::string >
{
public:
    typedef std::vector< std::string > list_t;
    typedef list_t::allocator_type allocator_t;
    
protected:
    std::vector< std::string > m_input_queries;
    
public:
    input_query( const allocator_t alloc ) noexcept;
};


/**
 * @brief The basic input logging object
 */
class basic_log_input {
public:
    typedef std::string input_flag_t;
    typedef std::string input_t;
    typedef std::pair< input_flag_t, input_t > input_resource_t;
    typedef std::map< input_flag_t, input_t > input_collection_t;

protected:
    basic_log& log;
    input_flag_t input_flag;
    input_t input_str;
    input_collection_t input_collection;
    
    void set_input_flag( const input_flag_t iflag );
    
public:
    /**
     * @brief Constructor
     * @param logger A basic_log based logger for log output
     */
    explicit basic_log_input( basic_log& logger );
    
    /**
     * @brief Clear the current buffer values
     */
    void clear();

    /**
     * @brief Reset the whole input collection and all flags.
     */
     void reset();
    
    /**
     * @brief Get the input for a specified key
     * @param key The input flag key to look for
     * @returns The input for key or an empty string, if the key does not exist in the input collection
     */
    const input_t get_input( const input_flag_t key ) const;
    
    /**
     * @brief Get the current input value
     * @returns The last input or an empty string, if there is no current input value
     */
    const input_t get_input_current() const;
    
	/**
	 * @brief Insert some function into this basic_log_input
	 * @param f A function getting a basic_log_input and returning a basic_log_input
	 */
	basic_log_input& operator>>(basic_log_input& (*f)(basic_log_input& l));
	
	/**
     * @brief Flag the next input with a string
     * @param f A function getting a basic_log_input and returning a basic_log_input
     */
    basic_log_input& operator>>( const logcpp::input_flag iflag );
    
	/**
	 * @brief Insert some object into this basic_log_input
	 * @param t Some object to log to this basic_log_input
	 */
    template< typename T >
	basic_log_input& operator>>(const T& t) {
        if ( ! log.is_new_record() ) {
            log.end_record();
        }
		log.log<T>(t);
		return *this;
	}
	
	/**
     * @brief Save the current input to the input collection, clear the input variables and end the log output record
     */
	void finalize();
    
    /**
     * @brief Wait for input from std::cin, finalize the input and end the current record
     */
    void input();
    
    /**
     * @brief Query a set of values
     * @param iquery_list The list of input flags that issue one value per flag
     * @returns A map by flag with the issued values
     */
    input_collection_t query_input_values( const input_query iquery );
};


/**
 * @brief Free function that asks for a user input
 * @param in_log A logger object providing the function input()
 */
template< typename logger_t >
inline logger_t& input(logger_t& in_log) {
	in_log.input();
	return in_log;
}

/**
 * @brief Free function that flushes the input data to the input collection
 * @param in_log A logger object providing the function finalize()
 */
template< typename logger_t >
inline logger_t& finalize(logger_t& in_log) {
	in_log.finalize();
	return in_log;
}

/**
 * @brief Template specialization for input and basic_log_input
 */
template basic_log_input& input(basic_log_input&);

/**
 * @brief Template specialization for finalize and basic_log_input
 */
template basic_log_input& finalize(basic_log_input&);

/**
 * @brief A simple logger with input functionality
 */
typedef basic_log_input in_logger;

} // namespace logcpp


/**
 * @def INPUT
 * @brief Insert a input into the log stream
 */
#define INPUT logcpp::input


/**
 * @def FINALIZE
 * @brief Insert a finalize into the log stream
 */
#define FINALIZE logcpp::finalize
