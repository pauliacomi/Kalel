#pragma once

#include <string>
#include <map>


struct http_response {

	// Response
	std::string                        status;						// used to transmit the server status code 202 OK/ 404 Not Found
	std::string						   http_version;				// Http version

	// Response headers
	std::string                        server;
	std::string                        date;
	std::string                        connection;
	std::string                        www_authentication;
	
	// Entity headers
	std::string						   content_type;
	std::string						   content_length;
	std::string						   content_encoding;

	// Response body
	std::string                        body;

	// Implementation-specific members

	/*   authentication_given is true when the user has entered a username and password.
	These can then be read from username and password */
	bool authentication_given = false;

	/*   disconnected is true when the client cannot process a request and terminates early. */
	bool disconnected = false;

	/*   error is true when there's an error on the line. */
	bool error = false;

	/*   error_str is set when the an error is seen.. */
	std::string error_str;
};
