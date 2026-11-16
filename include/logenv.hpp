// Copyright (C) 2026 The OnePointer Authors.
//

#pragma once

#include "logformat.hpp"

#include <map>
#include <string>

namespace logcpp {
namespace env {


class Value {
public:
    const std::string name;
    std::string value;

    Value();
    Value( const std::string vname, const std::string vval );

    Value& operator=( const Value& other );
    static Value& operator()();
};

class LogEnvironment
    :   protected std::map< std::string, Value >
{
public:
    LogEnvironment();

    Value& operator[]( const std::string key );
    Value& operator[]( const std::string key ) const;

    const std::string format( const LogFormat& formatter ) const;

    void clear();
};


} // namespace env
} // namespace logcpp