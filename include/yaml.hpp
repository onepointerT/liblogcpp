/**
 * @file yaml.hpp
 * @brief The YAML++ support library definition file
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
	LibLogC++: An intuitive and highly customizable LGPL library for logging with C++.
	Copyright (C) 2026 Sebastian Lau <lauseb644@gmail.com>


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

//#ifdef LOGCPP_YAML_SUPPORT

#include <initializer_list>
#include <list>
#include <map>
#include <string>

extern "C" {
#include "yaml.h"
}


#define LIBYAML_SCALAR_STYLE YAML_ANY_SCALAR_STYLE
#define LIBYAML_SEQUENCE_STYLE YAML_ANY_SEQUENCE_STYLE
#define LIBYAML_MAPPING_STYLE YAML_ANY_MAPPING_STYLE
#define LIBYAML_ENCODING YAML_UTF8_ENCODING
#define LIBYAML_LINEBREAK_STYLE YAML_LN_BREAK
#define LIBYAML_DEFAULT_NODE_TYPE YAML_SCALAR_NODE
#define LIBYAML_DEFAULT_EVENT_TYPE YAML_NO_EVENT


#define LIBYAML_VERSION_STRING std::string(yaml_get_version_string())
#define LIBYAML_VERSION_MINOR std::stoi(LIBYAML_VERSION_STRING.\
                                substr(0, LIBYAML_VERSION_STRING.find_first_of('.')-1))
#define LIBYAML_VERSION_MAJOR std::stoi(LIBYAML_VERSION_STRING.\
                                substr(LIBYAML_VERSION_STRING.find_first_of('.')+1,\
                                       LIBYAML_VERSION_STRING.find('.',\
                                            LIBYAML_VERSION_STRING.find_first_of('.')+1)\
                                        - LIBYAML_VERSION_STRING.find_first_of('.') + 1\
                                    ))

namespace logcpp {

// Forward-declaration
namespace env {
class Value;
} // namespace

namespace yaml {

inline static yaml_parser_t& yaml_parser
        = *new yaml_parser_t{ .encoding = LIBYAML_ENCODING };
#define LIBYAML_PARSER yaml_parser

inline static yaml_version_directive_t* yaml_version
        = new yaml_version_directive_t{ .major = LIBYAML_VERSION_MAJOR
                                      , .minor = LIBYAML_VERSION_MINOR
                    };

class YamlNode
{
public:
    const static yaml_scalar_style_t yaml_scalars = LIBYAML_SCALAR_STYLE;
    const static yaml_sequence_style_t yaml_sequences = LIBYAML_SEQUENCE_STYLE;
    const static yaml_mapping_style_t yaml_mappings = LIBYAML_MAPPING_STYLE;

    yaml_node_pair_t& node;
    yaml_node_t* yaml;
    std::list< YamlNode* > children;

    YamlNode( const yaml_node_t& yc_node );
    YamlNode( const yaml_node_pair_t* yc_node_pair = nullptr );
    YamlNode( const std::string name, const std::string value );

    void setKey( const std::string key = "" );
    void setValue( const std::string value = "" );

    env::Value& toEnvValue() const;
};

class YamlDocument
    :   public std::map< std::string, YamlNode* >
{
protected:
    std::string str_docdata;

public:
    const static yaml_encoding_t yaml_encoding = LIBYAML_ENCODING;
    const static yaml_break_t yaml_newline = LIBYAML_LINEBREAK_STYLE;

    const std::string docdata() const;
    
    yaml_document_t* doc;
    YamlNode* root_node;

    unsigned int toMap();
    unsigned int fromMap();
    
    YamlDocument( const std::string docdata );

    YamlDocument& operator<<( const env::Value& val );

    yaml_parser_t& parser;
    yaml_event_t& event;

    static yaml_document_t& initdoc( yaml_tag_directive_t* tag_directives_start
                                        = new yaml_tag_directive_t{ .handle = NULL, .prefix = NULL }
                                   , yaml_tag_directive_t* tag_directives_end
                                        = new yaml_tag_directive_t{ .handle = NULL, .prefix = NULL }
    );

    static unsigned int readdoc( const std::string docdata, YamlDocument& ydoc );

    static unsigned int savedoc( const yaml_document_t& document );

    void clear();
};



class YamlFile {
protected:
    std::fstream& _fh;
    const std::string _openmode;

public:
    YamlFile( const std::string path, const std::string openmode = "r" );
};


} // namespace yaml
} // namespace logcpp


// #endif // LOGCPP_YAML_SUPPORT