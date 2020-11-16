#pragma once

#include "Supplement.h"
#include <qthread.h>

extern bool isConn;
extern SOCKET server_socket;

/* bolcking server_socket, no need to SLEEP
#define POLLING_INTERVAL 2	// Sleep POLLING_INTERVAL secs between each recv actions
*/

class TRecvServer : public QThread
{
	Q_OBJECT

public:
	TRecvServer();
	~TRecvServer();

	void run();
	QString RecvStr(SOCKET sock, QString& info, char buf[]);

signals:
	void sig_recv_res(QString res, QString info, bool succ, bool sc);

};

