/**
 * @file logmanip.hpp
 * @brief Wrappers for inserting all functions from iomanip into a basic_log
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


#include <iomanip>

#include "basic_log.hpp"


#ifdef __unix__
extern logcpp::basic_log& operator<<(logcpp::basic_log&, std::_Resetiosflags (*resetiosflags)(std::ios_base::fmtflags));
extern logcpp::basic_log& operator<<(logcpp::basic_log&, std::_Setiosflags (*setiosflags)(std::ios_base::fmtflags));
extern logcpp::basic_log& operator<<(logcpp::basic_log&, std::_Setbase (*setbase)(int));
extern logcpp::basic_log& operator<<(logcpp::basic_log&, std::_Setfill<char> (*setfill)(char));
extern logcpp::basic_log& operator<<(logcpp::basic_log&, std::_Setprecision (*setprecision)(int));
extern logcpp::basic_log& operator<<(logcpp::basic_log&, std::_Setw (*setw)(int));

template < typename _MoneyT >
extern logcpp::basic_log& operator<<(logcpp::basic_log&, std::_Put_money<_MoneyT> (*put_money)(_MoneyT&, bool));

#endif
