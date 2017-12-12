#include "NetworkClient.h"

NetworkClient::NetworkClient() {
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSAData wsaData;

	int Error;
	if ((Error = WSAStartup(wVersionRequested, &wsaData)) != 0) {
		PrintError(Error);
	}
}

NetworkClient::~NetworkClient() {
	//Disconnect();
	//WSACleanup();
}


bool NetworkClient::Connect(char* Host, unsigned short Port) {

	if (TCPSocket != INVALID_SOCKET) {
		//Disconnect();
		closesocket(TCPSocket);
		TCPSocket = INVALID_SOCKET;
	}


	unsigned int RemoteAddress = GetIP(Host);
	if (RemoteAddress == INADDR_NONE) {
		return false;
	}


	int type = SOCK_STREAM;		//TCP
	//int type = SOCK_DGRAM;	//UDP

	TCPSocket = socket(AF_INET, type, 0);
	if (TCPSocket == INVALID_SOCKET) {
		PrintError(WSAGetLastError());
		return false;
	}


	sockaddr_in Remote;
	Remote.sin_family = AF_INET;
	Remote.sin_addr.s_addr = RemoteAddress;
	Remote.sin_port = htons(Port);

	if (connect(TCPSocket, (sockaddr*)&Remote, sizeof(Remote)) == SOCKET_ERROR) {
		PrintError(WSAGetLastError());

		//Disconnect functie aanroepen?
		closesocket(TCPSocket);
		TCPSocket = INVALID_SOCKET;
		
		return false;
	}


	//Set Non-blocking mode
	u_long iMode = 1;
	if (ioctlsocket(TCPSocket, FIONBIO, &iMode) != NO_ERROR) {
		PrintError(WSAGetLastError());
	}


	//No delay
	/* if (type == SOCK_STREAM) {
		unsigned int NoDelay = 1;
		setsockopt(TCPSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&NoDelay, sizeof(NoDelay));
	} */


	return true;
}

//void NetworkClient::Send(char* Message) {
void NetworkClient::Send(std::string Message, bool SendLength) {

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
	if (send(TCPSocket, Message.c_str(), Message.size(), 0) == SOCKET_ERROR) {
		PrintError(WSAGetLastError());
		return;
	}
}

std::string NetworkClient::Receive() {
	std::string Message;

	FD_SET ReadSet;
	FD_ZERO(&ReadSet);
	FD_SET(TCPSocket, &ReadSet);

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

		do {
			totalBytes = recv(TCPSocket, recvbuf, recvbuflen, 0);
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
		} while (totalBytes > 0);
	}

	return Message;
}

bool NetworkClient::Disconnect() {
	if (shutdown(TCPSocket, SD_SEND) == SOCKET_ERROR) {
		PrintError(WSAGetLastError());
		closesocket(TCPSocket);
		//WSACleanup();
		return false;
	}

	closesocket(TCPSocket);
	//WSACleanup();

	TCPSocket = INVALID_SOCKET;

	return true;
}


unsigned int NetworkClient::GetIP(char* Host) {
	unsigned int RemoteAddress = inet_addr(Host);
	if (RemoteAddress == INADDR_NONE) {
		// Host isn't a dotted IP, so resolve it through DNS

		hostent* HostEnt = gethostbyname(Host);
		if (HostEnt == 0) {
			return INADDR_NONE;
		}
		RemoteAddress = *(u_long*)HostEnt->h_addr_list[0];
	}
	return RemoteAddress;
}

void NetworkClient::PrintError(int Error) {

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