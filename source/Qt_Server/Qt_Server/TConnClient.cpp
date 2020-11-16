#include "TConnClient.h"

TConnClient::TConnClient() { }
TConnClient::~TConnClient() { }

void TConnClient::run()
{
	u_short cid = 1;
	while (true) {
		int len = sizeof(SOCKADDR);
		QString res;
		SOCKET accept_socket;
		SOCKADDR_IN accept_addr;

		// block, waiting for accept client
		accept_socket = accept(server_socket, (SOCKADDR*)&accept_addr, &len);

		if (accept_socket == SOCKET_ERROR) {
			res = "Failed: Connection";
			WSACleanup();
		}
		else {
			Client* pClient = new Client(cid, accept_socket, accept_addr, false);

			QString addr = inet_ntoa(accept_addr.sin_addr);
			QString port = QString::number(ntohs(accept_addr.sin_port));
			res = "NEW Connection: [" + QString::number(cid) + "] IP: " 
				+ addr + " Port: " + port;

			l_client.push_back(pClient);
			qDebug() << "Client conn:" << QString::number(l_client.size());

			cid++;
		}
		emit sig_conn_res(res);
	}
}

