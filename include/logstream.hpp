/**
 * @file logstream.hpp
 * @brief The underlying log stream buffer
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




#include <ostream>
#include <sstream>



namespace logcpp {

/**
 * @brief The stream buffer that buffers log records before writing until they are ended with logcpp::endrec
 */
class logstreambuf
	:	public std::ostream
{
protected:
	/**
	 * @brief The buffer of a log stream buffer
	 */
	class logbuffer
		:	public std::stringbuf
	{
    protected:
		std::ostream out;
	public:
		/**
		 * @brief The constructor
		 * @param outbuf A pointer to some streambuf where all buffered content is written to when flushed
		 */
		logbuffer(std::streambuf* outbuf)
			:	out( outbuf )
		{}
		
		virtual ~logbuffer() {}

		/**
		 * @brief Write the rest of the buffered content to the target stream and flush it
		 */
		virtual int sync() {
			out << str();
			str("");
			out.flush();
			return 0;
		}
		
		/**
		 * @brief Determines, if the underlying stream is a terminal
		 * @note Will fail in some cases where a terminal supports positioning
		 */
		bool sink_is_terminal() {
			return ( (int)out.tellp() == -1 );
		}
	};

	logbuffer buf;

public:
	/**
	 * @brief The constructor
	 * @param outbuf A pointer to some streambuf where all buffered content is written to when flushed
	 */
	logstreambuf( std::streambuf* outbuf )
		:	std::ostream(&buf),
			buf( outbuf )
	{}

	/**
	 * @brief Delete all unflushed content from the target stream and flush to nowhere
	 */
	void clear_buf() {
		buf.str("");
		flush();
	}

	/**
	 * @brief Get the content of the target stream buffer
	 * @return The content of the target stream buffer
	 */
	const std::string get_buf() const {
		return buf.str();
	}

	/**
	 * @brief Find out, wether this logstreambuf has content to flush in its target streambuf or not
	 * @return True, if content is found; false otherwise
	 */
	bool has_buffered_content() {
		return ( buf.str().length() > 0 );
	}
	
	/**
	 * @brief Determines, if the underlying stream is a terminal
	 * @note Will fail in some cases where a terminal supports positioning
	 */
	bool sink_is_terminal() {
		return buf.sink_is_terminal();
	}
};


inline logstreambuf& operator<<(  logstreambuf& out, const char* characters ) {
    std::istringstream input( characters );
    out << input.rdbuf();
    return out;
}


} // namespace logcpp


