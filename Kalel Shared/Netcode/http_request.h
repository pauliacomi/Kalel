#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#pragma once

#include <string>
#include <map>

struct http_request {

	const std::string authorization   = "Authorization: Basic ";
	const std::string accept          = "Accept: "             ;
	const std::string accept_language = "Accept-Language: "    ;
	const std::string accept_encoding = "Accept-Encoding: "    ;
	const std::string user_agent      = "User-Agent: "         ;

	struct method
	{
		const std::string get   = "GET";
		const std::string set	= "SET";
		const std::string post	= "POST";
		const std::string del	= "DEL";
	};

	http_request() : authentication_given_(false) {}

	
	/* method_: used to transmit the REST method desired
	o  GET
	o  POST
	and so on */
	std::string                        method_;

	
	/* path_: used to transmit the path:
	o  /api/rest */
	std::string                        path_;


	/* params_: used to transmit the parameters after the path
	o  /rest?something=1
	and so on */
	std::map<std::string, std::string> params_;

	
	std::string                        accept_;
	std::string                        accept_language_;
	std::string                        accept_encoding_;
	std::string                        user_agent_;

	
	/* status_: used to transmit server's error status, such as
	o  202 OK
	o  404 Not Found
	and so on */
	std::string                        status_;

	
	/* auth_realm_: allows to set the basic realm for an authentication,
	no need to additionally set status_ if set */
	std::string                        auth_realm_;

	
	std::string                        answer_;

	/*   authentication_given_ is true when the user has entered a username and password.
	These can then be read from username_ and password_ */
	bool authentication_given_;
	
	
	std::string username_;
	std::string password_;
};

#endif