#include "TRecvClient.h"

using namespace std;

TRecvClient::TRecvClient() { }
TRecvClient::~TRecvClient() { }

void TRecvClient::run()
{
	bool client_clo = false;
	bool recv_succ = false;
	char recv_buf[BUF_LEN];
	QString res, info, cid;
	Client* p = nullptr;
	list<Client *>::iterator it;
	
	while (true) {
		qDebug() << "Thread: RecvClient RUN";

		for (it = l_client.begin(); it != l_client.end(); ++it) {
			// get client obj
			p = *it;
			cid = QString::number(p->cid);
			// reset vars
			res.clear();
			info.clear();	
			client_clo = false;
			recv_succ = false;

			res = RecvStr(p->accept_socket, info, recv_buf);

			if (!res.isEmpty()) {	// receive data successfully
				recv_succ = true;
			}

			if (CLIENT_CLOSE == res) {	// signal: client closed.
				client_clo = true;
			}

			if (!info.isEmpty()) {	// info is empty: recv null data, no need to update UI
				emit sig_recv_res(res, info, cid, recv_succ, client_clo);
			}
		}
		qDebug() << "Thread: RecvClient SLEEP.";
		sleep(POLLING_INTERVAL);
	}
}

QString TRecvClient::RecvStr(SOCKET sock, QString& info, char buf[])
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
