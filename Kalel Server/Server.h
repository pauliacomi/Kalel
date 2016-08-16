#pragma once


class Server
{
public:
	Server();
	~Server();
	void sockCreate();
	void server();
	int client();
	void sockOpen();
	void sockClose(SOCKET sock);

private:
	SOCKET serverSocket = INVALID_SOCKET;
};

