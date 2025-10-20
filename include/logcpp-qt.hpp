/**
 * @file logcpp-qt.hpp
 * @brief Qt compatibility header
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
	LibLogC++: An intuitive and highly customizable LGPL library for logging with C++.
	Copyright (C) 2017 Sebastian Lau <lauseb644@gmail.com>


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


#include <QString>
#include "logstream.hpp"

namespace logcpp {

/**
 * @brief Conversion compliance function to be able to pass QString to a logstreambuf
 * @param out A sink of a logger
 * @param str A QString to log
 * @returns A reference to the sink
 */
inline logstreambuf& operator<<(logstreambuf& out, const QString& str) {
  out << str.toStdString();
  return out;
}


} // namespace logcpp
