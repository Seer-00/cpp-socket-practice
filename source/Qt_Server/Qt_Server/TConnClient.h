#pragma once
#ifndef T_CONNCLIENT_H
#define T_CONNCLIENT_H

#include <qthread.h>
#include "Client.h"

extern SOCKET server_socket;
extern SOCKADDR_IN server_addr;
extern std::list<Client *> l_client;

class TConnClient : public QThread
{
	Q_OBJECT

public:
	TConnClient();
	~TConnClient();

	void run();

signals:
	void sig_conn_res(QString s);

};



#endif // !T_CONNCLIENT_H

