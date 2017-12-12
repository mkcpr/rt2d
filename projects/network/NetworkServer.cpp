#include "NetworkServer.h"

NetworkServer::NetworkServer() {
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSAData wsaData;

	int Error;
	if ((Error = WSAStartup(wVersionRequested, &wsaData)) != 0) {
		PrintError(Error);
	}

	ListenSocket = INVALID_SOCKET;

	//std::cout << ListenThread.get_id();		//0

	//ClientSocket = INVALID_SOCKET;
}

bool NetworkServer::Start(unsigned short Port) {

	if (ListenSocket != INVALID_SOCKET) {
		//Disconnect();
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
	}

	int type = SOCK_STREAM;		//TCP
	//int type = SOCK_DGRAM;	//UDP

	ListenSocket = socket(AF_INET, type, 0);
	if (ListenSocket == INVALID_SOCKET) {
		PrintError(WSAGetLastError());
		return false;
	}


	//#define DEFAULT_PORT "27015"
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = type;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::ostringstream oss;
	oss << Port;

	// Resolve the local address and port to be used by the server
	if (getaddrinfo(NULL, oss.str().c_str(), &hints, &result) != 0) {
		PrintError(WSAGetLastError());
		freeaddrinfo(result);
		return false;
	}


	if (bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		PrintError(WSAGetLastError());
		freeaddrinfo(result);

		//Disconnect functie aanroepen?
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;

		return false;
	}

	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		PrintError(WSAGetLastError());
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		return false;
	}

	return true;
}

void NetworkServer::Listen() {

	while (true) {	//For threading only

		FD_SET ReadSet;
		FD_ZERO(&ReadSet);
		FD_SET(ListenSocket, &ReadSet);

		//FD_SET WriteSet;
		//FD_ZERO(&WriteSet);

		timeval TimeOut;
		TimeOut.tv_usec = 0;

		int totalBytes = 0;
		if ((totalBytes = select(0, &ReadSet, NULL, NULL, &TimeOut)) == SOCKET_ERROR) {
			PrintError(WSAGetLastError());
			return;
		}


		if (totalBytes > 0) {

			SOCKET ClientSocket = INVALID_SOCKET;

			// Accept a client socket
			if ((ClientSocket = accept(ListenSocket, NULL, NULL)) == INVALID_SOCKET) {
				PrintError(WSAGetLastError());

				//closesocket(ListenSocket);
				//return;
			}

			ClientConnection cc;
			cc.ClientSocket = ClientSocket;
			ClientConnections.push_back(cc);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));	//For threading only
	}
}

void NetworkServer::ListenASync() {
	std::thread ListenThread(&NetworkServer::Listen, this);
	ListenThread.detach();
}

void NetworkServer::HandleClients() {

	for (std::vector<ClientConnection>::iterator cc = ClientConnections.begin(); cc != ClientConnections.end(); ++cc) {
		std::string Message = Receive((*cc).ClientSocket);
		if (Message != "") {
			Send((*cc).ClientSocket, Message);
		}
		//Send((*cc).ClientSocket, "Hello");
	}
}

void NetworkServer::Send(SOCKET ClientSocket, std::string Message, bool SendLength) {

	//See: https://stackoverflow.com/questions/18670807/sending-and-receiving-stdstring-over-socket
	/* if (SendLength) {
	std::ostringstream dataLength;
	dataLength << htonl(Message.size());	// Ensure network byte order when sending the data length

	//std::cout << oss.str().c_str() << std::endl;
	//std::cout << oss.str().size() << std::endl;

	if (send(TCPSocket, dataLength.str().c_str(), dataLength.str().size(), 0) == SOCKET_ERROR) {
	PrintError(WSAGetLastError());
	return;
	}
	}*/

	//if (send(TCPSocket, Message, (int)strlen(Message), 0) == SOCKET_ERROR) {
	if (send(ClientSocket, Message.c_str(), Message.size(), 0) == SOCKET_ERROR) {
		PrintError(WSAGetLastError());
		return;
	}
}

std::string NetworkServer::Receive(SOCKET ClientSocket) {
	std::string Message;

	FD_SET ReadSet;
	FD_ZERO(&ReadSet);
	FD_SET(ClientSocket, &ReadSet);

	FD_SET WriteSet;
	//FD_ZERO(&WriteSet);

	timeval TimeOut;
	TimeOut.tv_usec = 0;

	int totalBytes = 0;
	if ((totalBytes = select(0, &ReadSet, NULL, NULL, &TimeOut)) == SOCKET_ERROR) {
		PrintError(WSAGetLastError());
		return "";
	}

	if (totalBytes > 0) {
#define DEFAULT_BUFLEN 512
		int recvbuflen = DEFAULT_BUFLEN;
		char recvbuf[DEFAULT_BUFLEN];

		//do {
			totalBytes = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (totalBytes > 0) {
				//printf("Bytes received: %d\n", totalBytes);

				Message.append(&(recvbuf[0]), totalBytes);
				recvbuf[0] = '\0';
			}
			else if (totalBytes == 0) {
				//Connection closed!
			}
			else {
				PrintError(WSAGetLastError());
			}
		//} while (totalBytes);
		//} while (totalBytes == DEFAULT_BUFLEN);
	}

	return Message;
}

bool NetworkServer::Stop() {
	return true;
}


void NetworkServer::PrintError(int Error) {

	switch (Error) {
		//WSAStartup
	case WSASYSNOTREADY:		std::cout << "WSASYSNOTREADY: The underlying network subsystem is not ready for network communication." << std::endl; break;
	case WSAVERNOTSUPPORTED:	std::cout << "WSAVERNOTSUPPORTED: The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation." << std::endl; break;
	case WSAEINPROGRESS:		std::cout << "WSAEINPROGRESS: A blocking Windows Sockets 1.1 operation is in progress." << std::endl; break;
	case WSAEPROCLIM:			std::cout << "WSAEPROCLIM: A limit on the number of tasks supported by the Windows Sockets implementation has been reached." << std::endl; break;
	case WSAEFAULT:				std::cout << "WSAEFAULT: The lpWSAData parameter is not a valid pointer." << std::endl; break;

		//WSASocket
		/* case WSANOTINITIALISED:
		case WSAENETDOWN:
		case WSAEAFNOSUPPORT:
		case WSAEFAULT:
		case WSAEINPROGRESS:
		//case WSAEINVAL:
		case WSAEINVALIDPROVIDER:
		case WSAEINVALIDPROCTABLE:
		case WSAEMFILE:
		case WSAENOBUFS:
		case WSAEPROTONOSUPPORT:
		case WSAEPROTOTYPE:
		case WSAEPROVIDERFAILEDINIT:
		case WSAESOCKTNOSUPPORT: */

		//connect
		//
	case WSAEADDRNOTAVAIL:		std::cout << "WSAEADDRNOTAVAIL: The remote address is not a valid address (such as INADDR_ANY or in6addr_any)." << std::endl; break;
		//
	case WSAECONNREFUSED:		std::cout << "WSAECONNREFUSED: The attempt to connect was forcefully rejected." << std::endl; break;
		//
	case WSAEISCONN:			std::cout << "WSAEISCONN: The socket is already connected (connection-oriented sockets only)." << std::endl; break;
	case WSAENETUNREACH:		std::cout << "WSAENETUNREACH: The network cannot be reached from this host at this time.." << std::endl; break;
	case WSAEHOSTUNREACH:		std::cout << "WSAEHOSTUNREACH: A socket operation was attempted to an unreachable host.." << std::endl; break;
		//
	case WSAETIMEDOUT:			std::cout << "WSAETIMEDOUT: An attempt to connect timed out without establishing a connection." << std::endl; break;
	case WSAEWOULDBLOCK:		std::cout << "WSAEWOULDBLOCK: The socket is marked as nonblocking and the connection cannot be completed immediately." << std::endl; break;
	case WSAEACCES:				std::cout << "WSAEACCES: An attempt to connect a datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled." << std::endl; break;



	default:					std::cout << "Unknown or not implemented error = " << WSAGetLastError() << std::endl; break;
	}
}