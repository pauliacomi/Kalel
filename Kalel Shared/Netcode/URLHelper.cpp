#include "../Forcelib.h"
#include "URLHelper.h"

#include "../stringHelpers.h"

#include <iostream>
#include <sstream>

URLHelper::URLHelper()
{
}


URLHelper::~URLHelper()
{
}

bool URLHelper::RemoveProtocolFromUrl(std::string const& url, std::string& protocol, std::string& rest) {

	std::string::size_type pos_colon = url.find(":");

	if (pos_colon == std::string::npos) {
		rest = url;
		return false;
	}

	if (url.size() < pos_colon + 2) {
		rest = url;
		return false;
	}

	if (url[pos_colon + 1] != '/' ||
		url[pos_colon + 2] != '/') {
		rest = url;
		return false;
	}

	protocol = url.substr(0, pos_colon);
	rest = url.substr(3 + pos_colon);  // Skipping three characters ( '://' )

	return true;
}

void URLHelper::SplitGetReq(std::string get_req, std::string& path, std::map<std::string, std::string>& params, std::string& http) {

	// Remove trailing newlines
	if (get_req[get_req.size() - 1] == '\x0d' ||
		get_req[get_req.size() - 1] == '\x0a')
		get_req = get_req.substr(0, get_req.size() - 1);

	if (get_req[get_req.size() - 1] == '\x0d' ||
		get_req[get_req.size() - 1] == '\x0a')
		get_req = get_req.substr(0, get_req.size() - 1);

	// Remove potential Trailing HTTP/1.x
	if (get_req.size() > 7) {
		if (get_req.substr(get_req.size() - 8, 5) == "HTTP/") {
			http = get_req.substr(get_req.size() - 8, 8);
			get_req = get_req.substr(0, get_req.size() - 9);
		}
	}

	std::string::size_type qm = get_req.find("?");
	if (qm != std::string::npos) {
		std::string url_params = get_req.substr(qm + 1);

		path = get_req.substr(0, qm);

		// Appending a '&' so that there are as many '&' as name-value pairs.
		// It makes it easier to split the url for name value pairs
		url_params += "&";

		std::string::size_type next_amp = url_params.find("&");

		while (next_amp != std::string::npos) {
			std::string name_value = url_params.substr(0, next_amp);
			url_params = url_params.substr(next_amp + 1);
			next_amp = url_params.find("&");

			std::string::size_type pos_equal = name_value.find("=");

			std::string nam = name_value.substr(0, pos_equal);
			std::string val = name_value.substr(pos_equal + 1);

			std::string::size_type pos_plus;
			while ((pos_plus = val.find("+")) != std::string::npos) {
				val.replace(pos_plus, 1, " ");
			}

			// Replacing %xy notation
			std::string::size_type pos_hex = 0;
			while ((pos_hex = val.find("%", pos_hex)) != std::string::npos) {
				std::stringstream h;
				h << val.substr(pos_hex + 1, 2);
				h << std::hex;

				int i;
				h >> i;

				std::stringstream f;
				f << static_cast<char>(i);
				std::string s;
				f >> s;

				val.replace(pos_hex, 3, s);
				pos_hex++;
			}

			params.insert(std::map<std::string, std::string>::value_type(nam, val));
		}
	}
	else {
		path = get_req;
	}
}


void URLHelper::SplitUrl(std::string const& url, std::string& protocol, std::string& server, std::string& path) {

	RemoveProtocolFromUrl(url, protocol, server);

	if (protocol == "http") {
		std::string::size_type pos_slash = server.find("/");

		if (pos_slash != std::string::npos) {
			path = server.substr(pos_slash);
			server = server.substr(0, pos_slash);
		}
		else {
			path = "/";
		}
	}
	else if (protocol == "file") {
		path = stringh::ReplaceInStr(server, "\\", "/");
		server = "";
	}
	else {
		std::cerr << "unknown protocol in SplitUrl: '" << protocol << "'" << std::endl;
	}
	
}

void URLHelper::BuildReq(std::string & request, std::string const & path, std::map<std::string, std::string> const & params)
{
	request += path;
	if (!params.empty())
	{
		request += "?";

		for (std::map<std::string, std::string>::const_iterator it = params.begin(); it != params.end(); ++it) {
			if (it != params.begin()) {
				request += "&";
			}
			request += it->first;
			request += "=";
			request += it->second;
		}
	}
}
