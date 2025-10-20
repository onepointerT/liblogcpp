/**
 * @file color_feature.hpp
 * @brief Functions and enums needed for controlling the style and colors of a terminal
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

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT

#include <string>
#include <vector>


namespace logcpp {

/**
 * @brief Enum that represents all terminal modes currently supported
 */
enum termmode {
	// Default colors
	col_white,
	col_cyan,
	col_magenta,
	col_blue,
	col_yellow,
	col_green,
	col_red,
	col_black,

	// Styles
	sty_bold,
	sty_unterline,
	sty_blink,
	sty_blink_off,
	
	// Control
	ctl_reset_col,
	ctl_reset_all,
	ctl_foreground,
	ctl_background
};

/**
 * @brief Class that implements the color feature for one logger
 */
class color_feature {
protected:
	bool m_background_ctl;
	unsigned int m_back;
	unsigned int m_fore;
	std::vector< unsigned int > m_styles;
    
public:
	/**
	 * @brief Constructor
	 */
	color_feature();
	
	/**
	 * @brief Get the CSI-Code representing the current mode
	 * @param mode The term mode
	 */
	const std::string code( termmode m );
	
	/**
	 * @brief Get the CSI-Code for one term mode only
	 * @param mode The term mode
	 */
	static const std::string code_plain( termmode m );
};



} // namespace logcpp


/**
 * @def COLOR
 * @brief Gets the code of the terminal mode specified without any additional modifications
 * @param col Terminal mode
 */
#define COLOR(col) logcpp::color_feature::code_plain(col)

#endif
