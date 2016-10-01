#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#pragma once

#include <string>
#include <map>

namespace http
{
	namespace responses
	{
		static const std::string success			= "200 Success";
		static const std::string ok					= "202 OK";
		static const std::string unauthorised		= "401 Unauthorised";
		static const std::string not_found			= "404 Not Found";
	};
}

struct http_response {

	const std::string header_server				= "Server: "			 ;
	const std::string header_date				= "Date: "				 ;
	const std::string header_connection			= "Connection: "		 ;
	const std::string header_content_type		= "Content-Type: "		 ;
	const std::string header_content_length		= "Content-Length: "	 ;
	const std::string header_www_authenticate	= "WWW - Authenticate: " ;

	

	http_response() : authentication_given_(false) {}


	/* status_: used to transmit server's error status, such as
	o  202 OK
	o  404 Not Found
	and so on */
	std::string                        status_;

	/* status_: used to transmit server's error status, such as
	o  202 OK
	o  404 Not Found
	and so on */
	std::string                        server_;
	std::string                        date_;
	std::string                        connection_;
	std::string                        content_type_;
	std::string                        content_length_;

	/* answer_: used to send the respose
	o  html
	o  json
	and so on */
	std::string                        answer_;

	/*   authentication_given_ is true when the user has entered a username and password.
	These can then be read from username_ and password_ */
	bool authentication_given_;
};

#endif