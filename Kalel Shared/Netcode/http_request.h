#pragma once

#include "Socket.h"
#include <string>
#include <map>

struct http_request {

	http_request() : authentication_given_(false) {}

	std::string                        method_;
	std::string                        path_;
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