/**
 * @file channel_log.hpp
 * @brief A logger with different channels
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

#include "basic_log.hpp"

#include <map>
#include <set>

namespace logcpp {

/**
 * @brief A logger with different channels of type logger_t
 */
template< class logger_t >
class channel_log {
protected:
	basic_log* m_default_channel;
	std::map< const std::string, logger_t* > m_channels;
	std::set< std::string > m_channels_enabled;

public:
	/**
	 * @brief Constructor.
	 */
	channel_log()
		:	m_default_channel( new basic_log() )
		,	m_channels()
		,	m_channels_enabled()
	{}

	/**
	 * @brief Destructor
	 */
	~channel_log() {}

	/**
	 * @brief Add a new logging channel with a given name
	 * @param channel_name Name of the new channel
	 * @param logger The logger that is to be added with that name
	 * @returns Wether the logger was added or not
	 */
	bool add_channel( const std::string channel_name, logger_t& logger ) {
		return m_channels.insert( std::make_pair(channel_name, &logger) ).second;
	}
	
	/**
	 * @brief Searches for a channel with a given name
	 * @param channel_name Name of the channel to find
	 * @returns Wether the channel was found.
	 */
	bool channel_exists( const std::string channel_name ) {
		bool found = false;
		for ( typename std::map< const std::string, logger_t* >::const_iterator it = m_channels.cbegin()
			; it != m_channels.cend()
			; it++
		) {
			if ( channel_name.compare(it->first) == 0 ) {
				found = true;
				break;
			}
		}
		return found;
	}
	
	/**
	 * @brief Enables a channel with a given name, if found
	 * @param channel_name Name of the channel to enable
	 * @returns Wether the channel is enabled. Returns false, if the channel is not found.
	 */
	bool enable_channel( const std::string channel_name ) {
		if ( !this->channel_exists(channel_name) ) {
			return false;
		}
		
		m_channels_enabled.insert( channel_name );
		return true;
	}
	
	/**
	 * @brief Disables a channel with a given name, if found
	 * @param channel_name Name of the channel to disable
	 */
	bool disable_channel( const std::string channel_name ) {
		m_channels_enabled.erase( channel_name );
		return true;
	}

	/**
	 * @brief This operator is used to access the channels by their name.
	 * @param channel_name The name of the channel to log to
	 * @returns A reference to the logger found or a default basic_log
	 * @note If no channel is found with this name, everything is logged to a basic_log on std::cout, no matter, if function are understood or not.
	 * @note Be careful not to have typos...
	 */
	logger_t& operator[] ( const std::string channel_name ) {
		try {
			logger_t* logger = m_channels.at( channel_name );
			return *logger;
		} catch ( std::out_of_range& oor ) {
			return *m_default_channel;
		}
	}
	
	/**
	 * @brief This operator is used to insert streams in all channels that are enabled
	 * @param t Content to log to all channels enabled at once
	 * @returns A reference to this
	 */
	template< typename T >
	channel_log<logger_t>& operator<< ( const T& t ) {
		for ( typename std::set< std::string >::const_iterator it = m_channels_enabled.cbegin()
			; it != m_channels_enabled.cend()
			; it++
		) {
			this->operator[](*it) << t;
		}
		return *this;
	}
	
	/**
	 * @brief This operator is used to insert functions in all channels that are enabled
	 * @param f Function to insert
	 * @returns A reference to this
	 */
	channel_log<logger_t>& operator<< ( logger_t& f(logger_t&) ) {
		for ( typename std::set< std::string >::const_iterator it = m_channels_enabled.cbegin()
			; it != m_channels_enabled.cend()
			; it++
		) {
			this->operator[](*it) << f;
		}
		return *this;
	}
};


/**
 * @brief A logger with different channels of type logger_t
 */
template< class logger_t >
using channellog = channel_log< logger_t >;

} // namespace logcpp

