/**
 * @file severity_feature.hpp
 * @brief Abstract template of a severity feature
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


namespace logcpp {

/**
 * @brief The abstract template of a severity feature with type severity_t
 * @note severity_t: The type of severity that is to be used
 */
template< typename severity_t >
class severity_feature
{
protected:
	/**
	 * @brief The maximum severity level that shall be logged
	 */
	severity_t max_severity_lvl;
	/**
	 * @brief The severity level currently used
	 */
	severity_t current_severity;

	/**
	 * @brief Constructor
	 * @param max_severity The maximum severity level that shall be logged
	 */
	explicit severity_feature( severity_t max_severity )
                :   max_severity_lvl( max_severity )
                ,   current_severity( max_severity )
	{}
	severity_feature( const severity_feature& ) = delete;

	/**
	 * @brief Find out if logging is enabled or not, depending on the current severity values
	 */
	bool log_enabled() const {
		return ( current_severity <= max_severity_lvl && max_severity_lvl != 0 ); // A severity of 0 should default to 'logging off'
	}

public:
	/**
	 * @brief Specify, how much output the Logger will produce
	 * @param severity The maximum severity level that shall be logged
	 */
	void set_max_severity_level(const severity_t severity ) {
		max_severity_lvl = severity;
	}

	/**
	 * @brief Get the current maximum severity level
	 */
	const severity_t severity_max() const { return max_severity_lvl; }
	
	/**
	 * @brief Get the current severity level
	 */
	const severity_t severity() const { return current_severity; }

};


} // namespace logcpp

