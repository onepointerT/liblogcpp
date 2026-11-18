/**
 * @file yaml.hpp
 * @brief The YAML++ support library implementing file
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

//#ifdef LOGCPP_YAML_SUPPORT

#include "yaml.hpp"

#include <fstream>

namespace logcpp {
namespace yaml {


YamlNode::YamlNode( const yaml_node_t& yc_node )
	:	node( *new yaml_node_pair_t{ .key = (int) std::stoi(*new std::string((char*) yc_node.tag), 0, 8)
								   , .value = (int) std::stoi(*new std::string((char*) yc_node.data.scalar.value), 0, 8)
		} )
	,	yaml( new yaml_node_t(yc_node) )
	,	children()
{}

YamlNode::YamlNode( const yaml_node_pair_t* yc_node_pair )
	:	node( (yc_node_pair != nullptr ? *new yaml_node_pair_t(*yc_node_pair) 
									  : *new yaml_node_pair_t{ .key = 0, .value = 0 }
		) )
	,	yaml( new yaml_node_t{   .tag = (yaml_char_t*) ((unsigned char*) yc_node_pair->key)
							   , .data = { .scalar = { .value = (yaml_char_t*) ((unsigned char*) yc_node_pair->value) } }
//							   , .type = yaml_node_type_t::YAML_SCALAR_NODE
		} )
	,	children()
{}

YamlNode::YamlNode( const std::string name, const std::string value )
	:	node( *new yaml_node_pair_t{ .key = (int) std::stoi(name, 0, 8)
								   , .value = (int) std::stoi(value, 0, 8)
		} )
	,	yaml( new yaml_node_t{   .tag = (yaml_char_t*) ((unsigned char*) name.c_str())
							   , .data = { .scalar = { .value = (yaml_char_t*) ((unsigned char*) value.c_str()) }}
//							   , .type = yaml_node_type_t::YAML_SCALAR_NODE
		} )
	,	children()	
{}

void YamlNode::setKey( const std::string key ) {
	this->node.key = (int) std::stoi(key, 0, 8);
	this->yaml->tag = (yaml_char_t*) ((unsigned char*) key.c_str());
}

void YamlNode::setValue( const std::string value ) {
	this->node.value = (int) std::stoi(value, 0, 8);
	this->yaml->data.scalar.value = (yaml_char_t*) ((unsigned char*) value.c_str());
}



const std::string YamlDocument::docdata() const {
	return this->str_docdata;
}


unsigned int YamlDocument::toMap() {

}

unsigned int YamlDocument::fromMap() {

}


YamlDocument::YamlDocument( const std::string docdata )
	:	str_docdata( docdata )
	,	doc( &(YamlDocument::initdoc()) )
	,	root_node( nullptr )
	,	parser( yaml::yaml_parser )
	,	event( *new yaml_event_t{ .type = YAML_NO_EVENT } )
{
	YamlDocument::readdoc( this->docdata(), *this );
}



yaml_document_t& YamlDocument::initdoc( yaml_tag_directive_t* tag_directives_start
									  , yaml_tag_directive_t* tag_directives_end )
{
	yaml_document_t& ydoc = *new yaml_document_t();
	yaml_document_initialize( &ydoc, yaml_version, tag_directives_start, tag_directives_end, 0, 0 );
	return ydoc;
}


unsigned int YamlDocument::readdoc( const std::string docdata, YamlDocument& ydoc ) {
	yaml_parser_set_input_string( &ydoc.parser, (unsigned char*) docdata.c_str(), docdata.length() );
	yaml_tag_directive_t* ytd_start = new yaml_tag_directive_t();
	yaml_tag_directive_t* ytd_end = new yaml_tag_directive_t();
	yaml_document_start_event_initialize( &ydoc.event, yaml_version, ytd_start, ytd_end, 0 );
	yaml_parser_set_input_string( &ydoc.parser, (unsigned char*) docdata.c_str(), docdata.length() );

	yaml_char_t* yc = NULL;
	yaml_char_t* yanchor = NULL;
	yaml_char_t* ytag = NULL;
	yaml_char_t* yvalue = NULL;
	int ylength = 0;
	yaml_alias_event_initialize( &ydoc.event, yc );
	yaml_scalar_event_initialize( &ydoc.event, yanchor, ytag, yvalue, ylength, 0, 0, LIBYAML_SCALAR_STYLE );
	yaml_document_start_event_initialize( &ydoc.event, yaml_version, ytd_start, ytd_end, 0 );
	yaml_document_end_event_initialize( &ydoc.event, 0 );

	//yaml_p
	yaml_parser_parse( &ydoc.parser, &ydoc.event );

	//yaml_do
}


unsigned int YamlDocument::savedoc( const yaml_document_t& document ) {

}


void YamlDocument::clear() {

}



} // namespace yaml
} // namespace logcpp

// #endif // LOGCPP_YAML_SUPPORT

