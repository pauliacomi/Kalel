#include "http_helpers.h"
#include "http_define.h"


std::string ParseMethod(const std::string & method)
{
	if (method == http::method::get) {		
		return http::method::get;
	}
	else if (method == http::method::post) {
		return http::method::post;
	}
	else if (method == http::method::put) {
		return http::method::put;
	}
	else if (method == http::method::del) {
		return http::method::del;
	}
	else
	{
		return std::string();
	}
}


std::string ParseStatusCode(const std::string & statuscode)
{
	if (statuscode == http::responses::not_found.substr(0, 3)) {
		return http::responses::not_found;
	}
	else if (statuscode == http::responses::ok.substr(0, 3)) {
		return http::responses::ok;
	}
	else if (statuscode == http::responses::unauthorised.substr(0, 3)) {
		return http::responses::unauthorised;
	}
	else if (statuscode == http::responses::no_content.substr(0, 3)) {
		return http::responses::no_content;
	}
	else
	{
		return std::string();
	}
}