#ifndef CLIENT_H
#define CLIENT_H

#define BUF_LEN 8 * 1024

#include "Supplement.h"

class Client
{
public:
	unsigned short cid;			// client ID
	SOCKET accept_socket;		// client socket
	SOCKADDR_IN accept_addr;	// client address

	Client();
	Client(unsigned short i, SOCKET s, SOCKADDR_IN addr, bool bk);
	~Client();

	static Client* findClientByCID(u_short cid);
};

#endif // !CLIENT_H
