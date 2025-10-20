#.rst:
# FindLibLogCPP
# -----------
#
# Try to find the LibLogCPP logging library
#
# Once done this will define
#
# ::
#
#   LIBLOGCPP_FOUND - System has LibLogCPP
#   LIBLOGCPP_INCLUDE_DIR - The LibLogCPP include directory
#   LIBLOGCPP_LIBRARY - The libraries needed to use LibLogCPP
#   LIBLOGCPP_VERSION_STRING - the version of LibLogCPP found (since CMake 2.8.8)
#
#
#	liblogcpp: A simple, but highly customizable and intuitive LGPL library for logging with C++.
#	Copyright (C) 2015-2021 Sebastian Lau
#
#	This library is free software; you can redistribute it and/or
#	modify it under the terms of the GNU Lesser General Public
#	License as published by the Free Software Foundation; either
#	version 3.0 of the License, or (at your option) any later version.
#
#	This library is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#	Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public
#	License along with this library; if not, write to the Free Software
#	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
#	USA
#


find_path(LIBLOGCPP_INCLUDE_DIR NAMES liblogcpp/log.hpp)
find_library(LIBLOGCPP_LIBRARY NAMES logcpp)
find_library(LIBLOGCPP_LIBRARY_STATIC NAMES logcpp.a)

if(LIBLOGCPP_INCLUDE_DIR AND EXISTS "${LIBLOGCPP_INCLUDE_DIR}/liblogcpp/logcppversion.hpp")
    file(STRINGS "${LIBLOGCPP_INCLUDE_DIR}/liblogcpp/logcppversion.hpp" liblogcpp_version_str
         REGEX "^#define[\t ]+LIBLOGCPP_DOTTED_VERSION[\t ]+\".*\"")

    string(REGEX REPLACE "^#define[\t ]+LIBLOGCPP_DOTTED_VERSION[\t ]+\"([^\"]*)\".*" "\\1"
           LIBLOGCPP_VERSION_STRING "${liblogcpp_version_str}")
    unset(liblogcpp_version_str)
endif()

# handle the QUIETLY and REQUIRED arguments and set LIBLOGCPP_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibLogCPP
                                  REQUIRED_VARS LIBLOGCPP_LIBRARY LIBLOGCPP_LIBRARY_STATIC LIBLOGCPP_INCLUDE_DIR
                                  VERSION_VAR LIBLOGCPP_VERSION_STRING)

mark_as_advanced(LIBLOGCPP_INCLUDE_DIR LIBLOGCPP_LIBRARY LIBLOGCPP_LIBRARY_STATIC)
