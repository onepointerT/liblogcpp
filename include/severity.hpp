/**
 * @file severity.hpp
 * @brief The abstract template for severities in LibLogC++
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

#include "config.hpp"

#include <string>
#include <array>


namespace logcpp {

/**
 * @brief Abstract description of a set of severities
 */
template< typename severity_t >
class AbstractSeverity {

	const std::array< const std::string, 1 + (int)severity_t::SEVERITY_SIZE >* m_names;
	unsigned int m_max_name_length;

protected:
	/**
	 * @brief Constructor
	 * @param severity_names A pointer to an array of names that describe the enum of severity_t
	 */
	AbstractSeverity( const std::array< const std::string, 1 + (int)severity_t::SEVERITY_SIZE >* severity_names )
		:	m_names( severity_names )
		,	m_max_name_length( 0 )
	{}

public:
	/**
	 * @return The name for severity_level as string
	 * @param lvl A severity_level to get as string
	 */
	const std::string severity_name( const severity_t lvl ) const {
		if( this->m_names == 0 ) {
			return std::string("NULL");
		} else if( static_cast< std::size_t >(lvl) < this->m_names->size() ) {
			return this->m_names->at(lvl);
		} else {
			return this->m_names->back();
		}
	}

	/**
	 * @return The amount of characters of the longest name of all severity_level
	 * @note Only important for alignment of severities
	 */
	const unsigned int max_name_length() {
		if ( m_max_name_length == 0 && m_names->size() > 1 ) {
			for ( size_t i = 0; i < m_names->size()-1; i++ ) { // SKIP last element (SIZE_T)
				if ( m_names[i].size() > m_max_name_length ) {
					m_max_name_length = m_names[i].size();
				}
			}
		}
		return m_max_name_length;
	}
};

} // namespace logcpp

