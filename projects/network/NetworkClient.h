#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <rt2d/core.h>

#include <winsock2.h>
//#include <ws2tcpip.h>
////#include <stdio.h>
//#pragma comment(lib, "Ws2_32.lib")

class NetworkClient {
public:
	NetworkClient();
	~NetworkClient();

	bool Connect(char* Host, unsigned short Port);
	
	//void Send(char* Message);
	void Send(std::string Message, bool SendLength = false);
	std::string Receive();

	bool Disconnect();

	unsigned int GetIP(char* Host);

private:
	SOCKET TCPSocket;
	void PrintError(int Error);
};

#endif /* NETWORKCLIENT_H */