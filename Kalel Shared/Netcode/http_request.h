#pragma once

#include <string>
#include <map>


struct http_request {

	// Request
	std::string                        method;					// used to transmit the HTTP method: GET/POST/etc
	std::string                        path;					// used to transmit the path: /api/rest
	std::map<std::string, std::string> params;					// used to transmit the parameters after the path ?something=1
	std::string                        http_version;			// HTTP version the request is based on

	// Request headers
	std::string                        accept;
	std::string                        accept_language;
	std::string                        accept_encoding;
	std::string                        user_agent;
	std::string                        authentication;
	std::string                        authentication_method;

	// Entity headers
	std::string						   content_type;
	std::string						   content_length;
	std::string						   content_encoding;

	// Request body
	std::string                        body;

	
	// Implementation-specific members
	
	std::string username;
	std::string password;
};