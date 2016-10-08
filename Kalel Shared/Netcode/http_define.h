#pragma once
#include <string>

namespace http
{

	namespace responses
	{			
		static const std::string cont				= "100 Continue"						 ;
		static const std::string switch_prot		= "101 Switching Protocols"				 ;
		static const std::string ok					= "200 OK"								 ;
		static const std::string created			= "201 Created"							 ;
		static const std::string accepted			= "202 Accepted"						 ;
		static const std::string non_auth_info		= "203 Non-Authoritative Information"	 ;
		static const std::string no_content			= "204 No Content"						 ;
		static const std::string reset_content		= "205 Reset Content"					 ;
		static const std::string partial_content	= "206 Partial Content"					 ;
		static const std::string mult_choices		= "300 Multiple Choices"				 ;
		static const std::string moved_perm			= "301 Moved Permanently"				 ;
		static const std::string found				= "302 Found"							 ;
		static const std::string see_other			= "303 See Other"						 ;
		static const std::string not_modif			= "304 Not Modified"					 ;
		static const std::string use_proxy			= "305 Use Proxy"						 ;
		static const std::string temp_redirect		= "307 Temporary Redirect"				 ;
		static const std::string bad_request		= "400 Bad Request"						 ;
		static const std::string unauthorised		= "401 Unauthorized"					 ;
		static const std::string payment_req		= "402 Payment Required"				 ;
		static const std::string forbidden			= "403 Forbidden"						 ;
		static const std::string not_found			= "404 Not Found"						 ;
		static const std::string method_not_allowed	= "405 Method Not Allowed"				 ;
		static const std::string not_acceptable		= "406 Not Acceptable"					 ;
		static const std::string proxy_auth_fail	= "407 Proxy Authentication Required"	 ;
		static const std::string req_timeout		= "408 Request Time-out"				 ;
		static const std::string conflict			= "409 Conflict"						 ;
		static const std::string gone				= "410 Gone"							 ;
		static const std::string length_req			= "411 Length Required"					 ;
		static const std::string precond_fail		= "412 Precondition Failed"				 ;
		static const std::string req_ent_toolarge	= "413 Request Entity Too Large"		 ;
		static const std::string req_uri_too_large  = "414 Request-URI Too Large"			 ;
		static const std::string unsupp_type		= "415 Unsupported Media Type"			 ;
		static const std::string range_unsat		= "416 Requested range not satisfiable"	 ;
		static const std::string expect_fail		= "417 Expectation Failed"				 ;
		static const std::string internal_err		= "500 Internal Server Error"			 ;
		static const std::string not_impl			= "501 Not Implemented"					 ;
		static const std::string bad_gateway		= "502 Bad Gateway"						 ;
		static const std::string serv_unav			= "503 Service Unavailable"				 ;
		static const std::string gateway_timeout	= "504 Gateway Time-out"				 ;
		static const std::string http_not_supp		= "505 HTTP Version not supported"		 ;
	}

	namespace method
	{
		static const std::string get			= "GET"						;
		static const std::string put			= "PUT"						;
		static const std::string post			= "POST"					;
		static const std::string del			= "DEL"						;
		static const std::string options		= "OPTIONS"					;
		static const std::string head			= "HEAD"					;
		static const std::string trace			= "TRACE"					;
		static const std::string connect		= "CONNECT"					;
	}

	namespace mimetype
	{
		static const std::string texthtml			= "text/html"				;
		static const std::string textplain			= "text/plain"				;
		static const std::string appjson			= "application/json"		;
		static const std::string appxml				= "application/xml"			;
	}

	namespace header
	{
		// request-specific
		const std::string accept				= "Accept: "				;
		const std::string accept_charset		= "Accept-Charset: "		;
		const std::string accept_language		= "Accept-Language: "		;
		const std::string accept_encoding		= "Accept-Encoding: "		;
		const std::string authorization			= "Authorization: "			;
		const std::string expect				= "Expect: "				;
		const std::string from					= "From: "					;
		const std::string host					= "Host: "					;
		const std::string match					= "Match: "					;
		const std::string if_modified			= "If-Modified: "			;
		const std::string if_none				= "If-None: "				;
		const std::string if_range				= "If-Range: "				;
		const std::string if_unmodified			= "If-Unmodified: "			;
		const std::string max_forwards			= "Max-Forwards: "			;
		const std::string proxy_authorization	= "Proxy-Authorization: "	;
		const std::string range					= "Range: "					;
		const std::string referer				= "Referer: "				;
		const std::string te					= "TE: "					;
		const std::string user_agent			= "User-Agent: "			;

		// response-specific
		const std::string accept_ranges			= "Accept-Ranges: "			;
		const std::string age					= "Age: "					;
		const std::string etag					= "ETag: "					;
		const std::string location				= "Location: "				;
		const std::string proxy_authenticate	= "Proxy-Authenticate: "	;
		const std::string retry_after			= "Retry-After: "			;
		const std::string server				= "Server: "				;
		const std::string vary					= "Vary: "					;
		const std::string www_authenticate		= "WWW-Authenticate: "		;

		// entity-specific
		const std::string allow					= "Allow: "					;
		const std::string content_encoding		= "Content-Encoding: "		;
		const std::string content_language		= "Content-Language: "		;
		const std::string content_length		= "Content-Length: "		;
		const std::string content_location		= "Content-Location: "		;
		const std::string content_md5			= "Content-MD5: "			;
		const std::string content_range			= "Content-Range: "			;
		const std::string content_type			= "Content-Type: "			;
		const std::string expires				= "Expires: "				;
		const std::string last_modified			= "Last-Modified: "			;

		// general header fields
		const std::string connection			= "Connection: "			;
		const std::string date					= "Date: "					;
		const std::string cache_control			= "Cache-Control: "			;
		const std::string pragma				= "Pragma: "				;
		const std::string trailer				= "Trailer: "				;
		const std::string transfer_encoding		= "Transfer-Encoding: "		;
		const std::string upgrade				= "Upgrade: "				;
		const std::string via					= "Via: "					;
		const std::string warning				= "Warning: "				;
	}

}