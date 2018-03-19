#pragma once
#include "header.h"


class Connection
{
	SOCKET Sock;
	SOCKET remoteSock;
	SOCKADDR_IN Param;
public:
	Connection();
	Connection(int nPort, char* nAddr);
	Connection(int nPort, ULONG nAddr);
	~Connection();
	
};
