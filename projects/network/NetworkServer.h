#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <rt2d/core.h>

#include <winsock2.h>
#include <ws2tcpip.h>		// for getaddrinfo
////#include <stdio.h>
//#pragma comment(lib, "Ws2_32.lib")

#include <thread>			//std::thread
#include <chrono>			//std::chrono::seconds

struct ClientConnection {
	SOCKET ClientSocket;
};

class NetworkServer {
public:
	NetworkServer();

	bool Start(unsigned short Port);
	
	void Listen();
	void ListenASync();

	void HandleClients();
	
	std::vector<ClientConnection> ClientConnections;
	void Send(SOCKET ClientSocket, std::string Message, bool SendLength = false);
	std::string NetworkServer::Receive(SOCKET ClientSocket);

	bool Stop();

private:

	SOCKET ListenSocket;
	void PrintError(int Error);

	std::thread ListenThread;
};

#endif /* NETWORKSERVER_H */