#pragma once
class Client
{
public:
	Client();
	~Client();
	int Run();
	void Close(SOCKET sock);
};

