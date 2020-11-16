#include "TRecvServer.h"

TRecvServer::TRecvServer() { }
TRecvServer::~TRecvServer() { }

void TRecvServer::run()
{
	bool server_close = false;
	bool recv_succ = false;
	char recv_buf[BUF_LEN];
	QString res, info;

	while (true) {
		qDebug() << "Thread: RecvServer RUN";

		if (isConn) {
			res.clear();
			info.clear();
			recv_succ = false;

			res = RecvStr(server_socket, info, recv_buf);

			if (!res.isEmpty()) {	// receive data successfully
				recv_succ = true;
			}

			// Server Closed.
			if (SCLOSED == res) {
				server_close = true;
			}

			if (!info.isEmpty()) {	// info is empty: recv null data, no need to update UI		
				emit sig_recv_res(res, info, recv_succ, server_close);
			} 

			if (server_close) {
				isConn = false;
				break;
			}
		}
		qDebug() << "Thread: RecvServer OVER.";
		// sleep(POLLING_INTERVAL);
		
		// sleep for debug
		sleep(5);
	}
	quit();
}

QString TRecvServer::RecvStr(SOCKET sock, QString & info, char buf[])
{
	int recv_len = 0;
	size_t data_len = 0;
	char head_buf[sizeof(size_t)];

	// receive: data_len
	recv_len = recv(sock, head_buf, sizeof(size_t), 0);

	if (recv_len == SOCKET_ERROR) {
		return "";
	}
	memcpy(&data_len, head_buf, sizeof(size_t));

	// receive: data
	recv_len = recv(sock, buf, data_len, 0);

	if (recv_len == SOCKET_ERROR) {
		info += "expect_data_len: " + QString::number(data_len) + "B";
		return "";
	}
	else {
		info += "recv_data_len: " + QString::number(recv_len) + "B";
		string s = buf;
		return str2qstr(s);
	}
}
