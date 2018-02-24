#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#pragma once

#include <string>
#include <map>


struct http_response {

	/* status_: used to transmit server's error status, such as
	o  202 OK
	o  404 Not Found
	and so on */
	std::string                        status_;
	std::string                        http_version_;

	std::string                        server_;
	std::string                        date_;
	std::string                        connection_;
	std::string                        content_type_;
	std::string                        content_length_;

	std::string                        answer_;

	/*   authentication_given_ is true when the user has entered a username and password.
	These can then be read from username_ and password_ */
	bool authentication_given_ = false;

	/*   disconnected_ is true when the client cannot process a request and terminates early. */
	bool disconnected_ = false;

	/*   error_ is true when there's an error on the line. */
	bool error_ = false;

	/*   error_str is set when the an error is seen.. */
	std::string error_str_;
};

#endif