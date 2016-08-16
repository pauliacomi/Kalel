#pragma once

class Server
{
public:
	Server();
	~Server();
	void Run();
	void sockClose(SOCKET sock);

private:
	SOCKET serverSocket = INVALID_SOCKET;
};

