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


#include "config.hpp"
#include "assert.hpp"
#include "basic_log_input.hpp"
#include "channel_log.hpp"
#include "severity_logger.hpp"
#include "logmanip.hpp"
#include "log.hpp"
#include "log_exception.hpp"
//#include "logcpp-qt.hpp"


int main(int argc, char** argv) {

    logcpp::severity_logger logger(logcpp::verbose);

    logger << "A Simple message" << logcpp::endrec;

    logger << std::setw(20) << "std::setw(20)" << logcpp::endrec;

    logger << "42 as hex:" << std::hex << 42 << logcpp::endrec;

    logger << "Message followed by std::endl" << logcpp::endl << "Message after std::endl, followed by logcpp::endrec" << logcpp::endrec;

    logger << "First message" << "Message following";

    logger << logcpp::endrec;

    logger << logcpp::debug << "And a debug message";

    logger << logcpp::critical << "A critical message" << logcpp::endrec;

    logger << "And another critical message" << logcpp::endrec;

    logger << logcpp::normal << SCOPE << "A normal message with SCOPE following the severity." << logcpp::endrec;

    logger << SCOPE << "A message with a SCOPE before" << logcpp::endrec;

    logger << SCOPE_SEVERITY(logcpp::error) << "A message calling SCOPE_SEVERITY(logcpp::error)" << logcpp::endrec;

	logcpp::assert< logcpp::severity_logger >( true
                                             , logger << logcpp::warning << "This assertion was evaluated true."
                                             , logger << "This will never happen due to hardcoded true." << logcpp::endrec );

    logcpp::assert< logcpp::severity_logger >( false
                                             , logger << logcpp::warning << "This assertion was evaluated false." << logcpp::endrec );

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
    logger << logcpp::ctl_background << logcpp::col_black << logcpp::sty_bold << logcpp::ctl_foreground << logcpp::col_yellow << "This is a message using the color feature." << logcpp::ctl_reset_col << logcpp::endrec;
#endif

 //   QString qstr("This is a QString.");
//    logger << qstr << logcpp::endrec;

    std::ofstream* ofs = new std::ofstream( "./flog_test.log", std::ofstream::out | std::ofstream::app | std::ofstream::ate );

    logcpp::logger flog( ofs->rdbuf() );

    flog << SCOPE << "This log goes to a file" << logcpp::endrec;

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
    flog << logcpp::col_red << "Tried to put colors to a file log here." << logcpp::endrec;
#endif


    logger << "Here we use the functionality of the channel logger" << logcpp::endrec;

    logcpp::channellog< logcpp::basic_log > ch;
    ch.add_channel( "file", flog );
    ch.add_channel( "console", logger );
    ch["file"] << "A message to the file channel" << logcpp::endrec;
    ch["console"] << "A message to the console channel" << logcpp::endrec;
	ch.enable_channel("console");
	ch.enable_channel("file");

	ch << "A message directly to all enabled channels at once" << logcpp::endrec;


    logcpp::severity_logger sflog( ofs->rdbuf() );
    sflog.set_critical_log_function(logcpp::abort_with_exception);

    try {
        sflog << logcpp::critical << SCOPE << "This is a second severity_logger to the same file" << logcpp::endrec;
    } catch( logcpp::critical_exception& ce ) {
        logger << "When using sflog with logcpp::critical, there was a logcpp::critical_exception thrown:" << ce.what() << logcpp::endl << "It was catched, but the next critical message through the global logger will use std::abort()..." << logcpp::endrec;
    }

    logcpp::globallog::enable_file_log();

    logcpp::globallog::get() << SCOPE << "This is the first message using the global logger." << logcpp::endrec;

    logcpp::globallog::disable_file_log();

    logcpp::globallog::get() << "This second global message was send after disabling the file log." << logcpp::endrec;

    if( !stdlog.file_log_enabled() ) {
        logcpp::globallog::enable_file_log();
    }

    logcpp::globallog::get() << "Re-enabled file log" << logcpp::endrec;

    logcpp::globallog::get().set_max_console_severity( logcpp::warning );

    logcpp::globallog::get() << logcpp::normal << "This message goes to the file only, because console only accepts critical messages." << logcpp::endrec;

    logcpp::globallog::get() << logcpp::warning << "Did you notice the previous global normal message? No? Max console severity is logcpp::warning..." << logcpp::endrec;

    stdlog << "This is sent with stdlog as macro for logcpp::globallog::get()" << logcpp::endrec;

    stdlog << logcpp::critical << "And again, 42 as hex via stdlog: " << std::hex << 42 << logcpp::endrec;

    stdlog << "And the current time";
#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
    stdlog << " in blue" << logcpp::col_blue;
#endif
    stdlog << ": " << logcpp::timestr() << logcpp::endrec;

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
    stdlog << "Now we set 'std::abort' as " << logcpp::col_black << logcpp::ctl_background << logcpp::col_red << "critical" << logcpp::ctl_reset_all << " log function and write a critical message again." << logcpp::endrec;
#else
    stdlog << "Now we set 'std::abort' as critical log function and write a critical message again." << logcpp::endrec;
#endif

    stdlog.set_critical_log_function(std::abort);

    stdlog << logcpp::critical << "That's it for now (aborted with critical log function)." << logcpp::endrec;


    lg << logcpp::normal << "In this example we will show how to handle user input." << logcpp::endrec;

    logcpp::basic_log_input lg_in( lg );

    lg_in >> "Firstly, ask for a simple input value by passing a 'logcpp::input_flag' question to the log." << logcpp::endl;
    lg_in >> "What is the current lesson on liblogcpp?" >> logcpp::input_flag("current_first") >> logcpp::input();

    basic_log_input::input_t input_current = lv_in.get_input_current();
    lg << "Your answer was: " << input_current << logcpp::endrec;

    lg_in.clear();
    lg << logcpp::endl << "Next, get a collection of input values by asking once..." << logcpp::endrec;

    logcpp::input_query iquery { "project_name", "project_version", "project_owner" };
    lg_in >> "To create a simple project, please answer a few questions.";
    logcpp::basic_log_input::input_collection_t in_collection = lg_in.query_input_values( iquery );

    lg << "You provided the following values: { 'project_name': '" << lg_in.get_input("project_name") << "', 'project_version': '";
    lg << lg_in.get_input("project_version") << "', 'project_owner': '" << lg_in.get_input("project_owner") << "' }.";
    lg << logcpp::endrec;

    lg << logcpp::endl << "Finished." << logcpp::endrec;

    return 0;
}
