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

#include "logenv.hpp"

#include <fstream>

namespace logcpp {
namespace yaml {


YamlNode::YamlNode( const yaml_node_t& yc_node )
	:	node( *new ::yaml_node_pair_t{ .key = (int) std::stoi(*new std::string((char*) yc_node.tag), 0, 8)
								   , .value = (int) std::stoi(*new std::string((char*) yc_node.data.scalar.value), 0, 8)
		} )
	,	yaml( new ::yaml_node_t(yc_node) )
	,	children()
{}

YamlNode::YamlNode( const yaml_node_pair_t* yc_node_pair )
	:	node( (yc_node_pair != nullptr ? *new ::yaml_node_pair_t(*yc_node_pair) 
									  : *new ::yaml_node_pair_t{ .key = 0, .value = 0 }
		) )
	,	yaml( new ::yaml_node_t{   .tag = (yaml_char_t*) ((unsigned char*) ((char*) yc_node_pair->key))
							   , .data = { .scalar = { 
												.value = (yaml_char_t*) ((unsigned char*) ((char*) yc_node_pair->value)),
												.length = strlen((char*) this->yaml->data.scalar.value),
												.style = LIBYAML_SCALAR_STYLE
											} }
//							   , .type = ::yaml_node_type_e::YAML_SCALAR_NODE
		} )
	,	children()
{}

YamlNode::YamlNode( const std::string name, const std::string value )
	:	node( *new ::yaml_node_pair_t{ .key = (int) std::stoi(name, 0, 8)
								   , .value = (int) std::stoi(value, 0, 8)
		} )
	,	yaml( new ::yaml_node_t{   .tag = (yaml_char_t*) ((unsigned char*) name.c_str())
							   , .data = { .scalar = { 
												.value = (yaml_char_t*) ((unsigned char*) value.c_str()),
												.length = strlen((char*) this->yaml->data.scalar.value),
												.style = LIBYAML_SCALAR_STYLE
											} }
//							   , .type = ::yaml_node_type_t::YAML_SCALAR_NODE
		} )
	,	children()	
{}

yaml_node_t& setNode( const yaml_node_pair_t& nd, yaml_node_t* node ) {
	if ( node == nullptr ) {
		node = new yaml_node_t{};
		node->type = yaml_node_type_t::YAML_SCALAR_NODE;
	}

	node->tag = (unsigned char*) ((char*) nd.key);
	node->data.scalar.value = (unsigned char*) ((char*) nd.value);
	node->data.scalar.length = strlen((char*) node->data.scalar.value);
	node->data.scalar.style = LIBYAML_SCALAR_STYLE;

	return *node;
}

void YamlNode::setKey( const std::string key ) {
	this->node.key = (int) std::stoi(key, 0, 8);
	this->yaml = &( YamlNode::setNode(this->node, this->yaml) );
}

void YamlNode::setValue( const std::string value ) {
	this->node.value = (int) std::stoi(value, 0, 8);
	this->yaml = &( YamlNode::setNode(this->node, this->yaml) );
}

const std::string YamlNode::getKey() const {
	return (char*) ((unsigned char*) ((char*) this->node.key));
}

const std::string YamlNode::getValue() const {
	return (char*) ((unsigned char*) ((char*) this->node.value));
}

env::Value& YamlNode::toEnvValue() const {
	return *new env::Value( this->getKey(), this->getValue() );
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
	,	event( *new ::yaml_event_t{ .type = YAML_NO_EVENT } )
	,	emitter( *new ::yaml_emitter_t{ .encoding = LIBYAML_ENCODING
									, .line_break = LIBYAML_LINEBREAK_STYLE
									, .document = this->doc
							} )
{
	::yaml_emitter_initialize( &this->emitter );
	YamlDocument::readdoc( this->docdata(), *this );
	this->root_node = new YamlNode( *::yaml_document_get_root_node( this->doc ) );
}



yaml_document_t& YamlDocument::initdoc( ::yaml_tag_directive_t* tag_directives_start
									  , ::yaml_tag_directive_t* tag_directives_end )
{
	::yaml_document_t& ydoc = *new ::yaml_document_t();
	::yaml_document_initialize( &ydoc, yaml_version, tag_directives_start, tag_directives_end, 0, 0 );
	return ydoc;
}


int YamlDocument::_callback_onEvent( ::yaml_parser_t* parser, ::yaml_event_t* event ) {

	if ( parser == NULL || event == NULL ) return 0;

	env::Value* yval = nullptr;
	YamlNode* ynode = nullptr;

	::yaml_event_type_t type;
	type = event->type;

	if (type == YAML_NO_EVENT)
		printf("???\n");
	else if (type == YAML_STREAM_START_EVENT)
		printf("+STR\n");
	else if (type == YAML_STREAM_END_EVENT)
		printf("-STR\n");
	else if (type == YAML_DOCUMENT_START_EVENT) {
		printf("+DOC");
		if (!event->data.document_start.implicit)
			printf(" ---");
		printf("\n");
	}
	else if (type == YAML_DOCUMENT_END_EVENT) {
		printf("-DOC");
		if (!event->data.document_end.implicit)
			printf(" ...");
		printf("\n");
	}
	else if (type == YAML_MAPPING_START_EVENT) {
		printf("+MAP");
		if (event->data.mapping_start.anchor)
			printf(" &%s", event->data.mapping_start.anchor);
		if (event->data.mapping_start.tag)
			printf(" <%s>", event->data.mapping_start.tag);
		printf("\n");
	}
	else if (type == YAML_MAPPING_END_EVENT)
		printf("-MAP\n");
	else if (type == YAML_SEQUENCE_START_EVENT) {
		printf("+SEQ");
		if (event->data.sequence_start.anchor)
			printf(" &%s", event->data.sequence_start.anchor);
		if (event->data.sequence_start.tag)
			printf(" <%s>", event->data.sequence_start.tag);
		printf("\n");
	}
	else if (type == YAML_SEQUENCE_END_EVENT)
		printf("-SEQ\n");
	else if (type == YAML_SCALAR_EVENT) {
		printf("=VAL");
		if (event->data.scalar.anchor)
			printf(" &%s", event->data.scalar.anchor);
		if (event->data.scalar.tag)
			printf(" <%s>", event->data.scalar.tag);
		switch (event->data.scalar.style) {
		case YAML_PLAIN_SCALAR_STYLE:
			printf(" :");
			break;
		case YAML_SINGLE_QUOTED_SCALAR_STYLE:
			printf(" '");
			break;
		case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
			printf(" \"");
			break;
		case YAML_LITERAL_SCALAR_STYLE:
			printf(" |");
			break;
		case YAML_FOLDED_SCALAR_STYLE:
			printf(" >");
			break;
		case YAML_ANY_SCALAR_STYLE:
			abort();
		}
//		print_escaped(event->data.scalar.value, event->data.scalar.length);
		printf("\n");
	}
	else if (type == YAML_ALIAS_EVENT)
		printf("=ALI *%s\n", event->data.alias.anchor);
	else
		abort();

//	yaml_event_delete(event);

	if (type == YAML_STREAM_END_EVENT)
		return 1;
	
	if ( ynode == nullptr )
		return 0;
	else YamlDocument::newNodes.emplace( ynode );
	
	if (!::yaml_parser_parse(parser, event)) {
		fprintf(stderr, "Parse error: %s\n", parser->problem);
		return 0;
	}

	return 1;
}


unsigned int YamlDocument::readdoc( const std::string docdata, YamlDocument& ydoc ) {
	if (!::yaml_parser_initialize(&ydoc.parser)) {
        return 0;
    }
	
	::yaml_parser_set_input_string( &ydoc.parser, (unsigned char*) docdata.c_str(), docdata.length() );

	::yaml_stream_callback_register( &YamlDocument::_callback_onEvent );

	unsigned int nfound = 0;
	while ( ydoc.event.type != YAML_STREAM_END_EVENT ) {
		::yaml_parser_parse( &ydoc.parser, &ydoc.event );

		if ( YamlDocument::newNodes.size() > 0 ) {
			while ( YamlDocument::newNodes.size() != 0 ) {
				YamlNode* no = YamlDocument::newNodes.front();
				*this << *no;
				YamlDocument::newNodes.pop();
			}
		}

		++nfound;
	}

	return nfound;
}


unsigned int YamlDocument::savedoc( const ::yaml_document_t& document ) {

}


void YamlDocument::clear() {

}



} // namespace yaml
} // namespace logcpp

// #endif // LOGCPP_YAML_SUPPORT

