#pragma once

#include <qthread.h>
#include "Client.h"

#define CLIENT_CLOSE "Client CLOSED."
#define POLLING_INTERVAL 2	// Sleep POLLING_INTERVAL secs between each recv actions

extern std::list<Client *> l_client;

class TRecvClient : public QThread
{
	Q_OBJECT

public:
	TRecvClient();
	~TRecvClient();

	void run();
	QString RecvStr(SOCKET sock, QString& info, char buf[]);

signals:
	void sig_recv_res(QString res, QString info, QString cid, bool succ, bool cclo);

};

