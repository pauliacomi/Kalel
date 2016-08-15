#pragma once
class Server
{
public:
	Server();
	~Server();
	int sockOpen(SOCKET sock);
	int sockClose(SOCKET sock);
};

