#include <Network.h>


Connection::Connection()
{
	Sock = 0;
	remoteSock = 0;
	ZeroMemory(&Param, sizeof(SOCKADDR_IN));
	Param.sin_family = AF_INET;
}

Connection::Connection(int nPort, char* nAddr)
{
	Sock = 0;
	remoteSock = 0;
	Param.sin_addr.S_un.S_addr = inet_addr(nAddr);
	Param.sin_port = nPort;
	Param.sin_family = AF_INET;
}

Connection::Connection(int nport, ULONG nAddr)
{
	remoteSock = 0;
	Sock = 0;
	Param.sin_addr.S_un.S_addr = htonl(nAddr);
	Param.sin_port = nport;
	Param.sin_family = AF_INET;
}

