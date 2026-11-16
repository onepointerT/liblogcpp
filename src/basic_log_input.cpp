/**
 * @file basic_log_input.cpp
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


#include "basic_log_input.hpp"

#include "basic_log.hpp"


namespace logcpp {


input_flag::input_flag( const std::string iflag )
    :   in_flag( iflag )
{}

const std::string input_flag::operator()() const {
    return in_flag;
}




input_query::input_query( const input_query::allocator_t alloc ) noexcept
    :   std::map< const std::string, std::string >()
{}




void basic_log_input::set_input_flag( const input_flag_t iflag ) {
    input_flag = iflag;
}


basic_log_input::basic_log_input( basic_log &logger )
    :   log( logger )
    ,   input_flag()
    ,   input_str()
    ,   input_collection()
{}


void basic_log_input::clear() {
    input_flag.clear();
    input_str.clear();
}


void basic_log_input::reset() {
    input_flag.clear();
    input_str.clear();
    input_collection.clear();
}


const basic_log_input::input_t basic_log_input::get_input( const input_flag_t key ) const {
    try {
        return input_collection.at( key );
    } catch ( std::out_of_range& oor ) {
        return input_t("");
    }
}


const basic_log_input::input_t basic_log_input::get_input_current() const {
    return input_str;
}


basic_log_input& basic_log_input::operator>>( basic_log_input& (*f)(basic_log_input& l)) {
    return f(*this);
}


basic_log_input& basic_log_input::operator>>( const logcpp::input_flag iflag ) {
    set_input_flag( iflag() );
    return *this;
}


void basic_log_input::finalize() {
    input_flag_t iflag = input_flag;
    if ( iflag.length() == 0 ) {
        iflag = std::to_string( input_collection.size() );
    }

    if ( input_str.length() > 0 ) {
        input_resource_t ires = input_resource_t( iflag, input_str );

        input_collection.insert( ires );

        log.log<std::string>( input_str );

        input_str.clear();
        input_flag.clear();
    }

    log.end_record();
}


void basic_log_input::input() {
    if ( input_str.length() == 0 ) {
        std::cin >> input_str;
        finalize();
        log.end_record();
    } else {
        finalize();
    }
}


basic_log_input::input_collection_t  basic_log_input::query_input_values( const input_query iquery ) {
    input_collection_t result;

    clear();

    for ( typename input_query::const_iterator iqry : { iquery.cbegin(), iquery.cend() } ) {
        std::string iflag = iqry->second;

        log << iflag << " [] ~> ";
        log.flush();

        set_input_flag( iflag );
        std::cin >> input_str;

        finalize();
        log.end_record();

        input_collection_t::const_reverse_iterator last_element = input_collection.crbegin();
        result.insert( std::make_pair( last_element->first, last_element->second ) );
    }


    return result;
}



} // namespace logcpp


