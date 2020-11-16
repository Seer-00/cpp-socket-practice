#include "Client.h"

using namespace std;

list<Client *> l_client;	// global list of clients

/* ------------- Constructor && Destructor ------------- */

Client::Client() { }

Client::Client(unsigned short i, SOCKET s, SOCKADDR_IN addr, bool blocking)
	: cid(i), accept_socket(s), accept_addr(addr)
{
	if (!blocking) {	// set non-blocking
		u_long ul = 1;
		int ret = ioctlsocket(accept_socket, FIONBIO, (unsigned long *)&ul);
		if (ret == SOCKET_ERROR) {
			qDebug() <<  "Failed: Set non-blocking. CID: " + QString::number(i);
		}
	}	
}

Client::~Client() { closesocket(accept_socket); }

/*------------- Functions ------------- */

Client * Client::findClientByCID(u_short cid)
{
	for (list<Client *>::iterator it = l_client.begin(); it != l_client.end(); ++it) {
		if ((*it)->cid == cid) {
			return *it;
		}
	}
	return nullptr;
}
