
project( liblogcpp LANGUAGES CXX )

cmake_minimum_required( VERSION 3.20 )

set( LIBLOGCPP_STABLE_VERSION 1 )
set( LIBLOGCPP_RELEASE 11 )
set( LIBLOGCPP_PATCH_VERSION 0-dev2 )
set( LIBLOGCPP_VERSION "${LIBLOGCPP_STABLE_VERSION}.${LIBLOGCPP_RELEASE}.${LIBLOGCPP_PATCH_VERSION}" )

set ( PROJECT_NAME "${CMAKE_PROJECT_NAME}" )
set ( PROJECT_VERSION "${LIBLOGCPP_VERSION}" )
set ( PROJECT_DESCRIPTION "An intuitive and highly customizable LGPL library for logging with C++." )

set( CMAKE_CXX_STANDARD 23 )


option( BUILD_LOGCPP_TEST "Build the test executables for logcpp." OFF )
option( BUILD_WITH_YAML_SUPPORT "Build with support for yaml, this means \
									you can serialize log input and/or pre-define \
									your queries and messages dynamically to and from *.yaml files." ON )
option( BUILD_WITH_COLOR_SUPPORT "Enable color support for logcpp." ON )
option( LEAVE_SCOPE_DIR_PREFIX "Let paths not to be abbreviated" OFF )

if( NOT DEFINED BUILD_WITH_YAML_SUPPORT )
	set( BUILD_WITH_YAML_SUPPORT ON )
	add_definitions( -fpermissive )
endif()
if( NOT DEFINED BUILD_WITH_COLOR_SUPPORT )
	set( BUILD_WITH_COLOR_SUPPORT ON )
endif()

if ( BUILD_WITH_COLOR_SUPPORT EQUAL ON )
	add_definitions( -DLOGCPP_ENABLE_COLOR_SUPPORT=1 )
endif()
if ( BUILD_WITH_YAML_SUPPORT )
	add_compile_definitions( LOGCPP_YAML_SUPPORT )
endif()
if ( LEAVE_SCOPE_DIR_PREFIX EQUAL ON )
	add_definitions( -DLOGCPP_LEAVE_SCOPE_DIRS_PREFIX=1 )
endif()

if( LOGCPP_DESTDIR )
else()
	set( LOGCPP_DESTDIR "${CMAKE_INSTALL_PREFIX}" )
endif()


if ( BUILD_WITH_YAML_SUPPORT )
	include_directories( lib/libyaml/include )
	add_subdirectory( lib/libyaml )
endif()



file (GLOB LIBLOGCPP_HEADERS ${PROJECT_SOURCE_DIR}/include/*.hpp )
file (GLOB LIBLOGCPP_SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp )
include_directories( ${PROJECT_SOURCE_DIR}/include )


if( LOGCPP_SHARED EQUAL ON )
	add_library( logcpp SHARED ${LIBLOGCPP_SOURCE} )
else()
	add_library( logcpp STATIC ${LIBLOGCPP_SOURCE} )
endif()
if ( BUILD_WITH_YAML_SUPPORT EQUAL ON )
	target_link_libraries( logcpp yaml )
endif()

if( BUILD_LOGCPP_TEST EQUAL ON )
    # Instruct CMake not to run moc automatically when needed.
	# set(CMAKE_AUTOMOC OFF)
	# Find the QtCore library
	# find_package(Qt5Core)
	# Build the test binary
	add_executable( logcpp_test ${PROJECT_SOURCE_DIR}/examples/main.cpp )
	target_link_libraries( logcpp_test logcpp )

	add_executable( logcpp_test2 ${PROJECT_SOURCE_DIR}/examples/main2.cpp )
	target_link_libraries( logcpp_test2 logcpp )
endif()

if( LOGCPP_HEADER_INSTALL_DIR )
else()
	set( LOGCPP_HEADER_INSTALL_DIR ${LOGCPP_DESTDIR}/include/liblogcpp )
endif()

if ( LOGCPP_LIB_INSTALL_DIR )
else()
	set ( LOGCPP_LIB_INSTALL_DIR ${LOGCPP_DESTDIR}/lib )
endif()

if( LOGCPP_INSTALL_LIBS )
	install(TARGETS logcpp DESTINATION ${LOGCPP_LIB_INSTALL_DIR} )
	install(FILES ${LIBLOGCPP_HEADERS} DESTINATION ${LOGCPP_HEADER_INSTALL_DIR} )
	install(FILES ${PROJECT_SOURCE_DIR}/lib/LibLogCPPConfig.cmake DESTINATION "${LOGCPP_LIB_INSTALL_DIR}/cmake/LibLogCPP" )
	configure_file(${PROJECT_SOURCE_DIR}/lib/liblogcpp.pc.in ${CMAKE_CURRENT_BINARY_DIR}/liblogcpp.pc @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/liblogcpp.pc DESTINATION "${LOGCPP_LIB_INSTALL_DIR}/pkgconfig" )
endif()


find_package(Doxygen)
if(DOXYGEN_FOUND)
	message("-- Enabled doc_logcpp target for make")
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc_logcpp ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating documentation with Doxygen")
endif()
