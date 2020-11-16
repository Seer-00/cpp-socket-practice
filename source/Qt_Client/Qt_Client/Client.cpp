#include "Client.h"

SOCKET server_socket;
SOCKADDR_IN server_addr;
bool isConn = false;
string ip = LOCAL_IP;
int port = LOCAL_PORT;

/* ------------- Constructor && Destructor ------------- */

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowTitle("Client");

	ui.e_input->setFocus();		// set focus on inputline
	// set shortcut: Enter == (pushBtn) Send
	connect(ui.e_input, SIGNAL(returnPressed()), ui.pb_send,
		SIGNAL(clicked()), Qt::UniqueConnection);

	// set font
	// QFont font("ZYSong18030", 12);
	QFont font("Microsoft YaHei", 10);
	ui.tb_info->setFont(font);
	ui.tb_content->setFont(font);

	// init
	Init();

	// Thread: recv server
	trs = new TRecvServer();
	connect(trs, SIGNAL(sig_recv_res(QString, QString, bool, bool)),
		this, SLOT(updateRecvStatus(QString, QString, bool, bool)));
}

Client::~Client()
{
	trs->quit();		// stop thread: trs
	Sleep(2 * 1000);	// sleep 2 *1000 ms for send "Client CLOSED."

	closesocket(server_socket);
	WSACleanup();
}

/* ------------- Initialize && Close Functions ------------- */

void Client::Init()
{
	// init WSA
	if (!InitWSA()) {
		QMessageBox::warning(this, "Warning", "Initialize WSA Failed.");
		exit(-1);
	}

	// definition of server_socket must be set AFTER "initWSA()"
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

}

bool Client::InitWSA()
{
	WORD w_req = MAKEWORD(2, 2);	// wVersionRequired, request: windows socket == 2.2
	WSADATA wsadata;				// store windows socket initialization info

	if (WSAStartup(w_req, &wsadata) != 0) {
		qDebug() << "Failed: initialized WSA";
		return false;
	}
	else {
		ui.tb_info->append("Success: initialize WSA");
	}

	// check socket version ?= 2.2
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		qDebug() << "Failed: incorrect WSA version";
		WSACleanup();
		return false;
	}
	else {
		ui.tb_info->append("Success: WSA version 2.2");
	}
	return true;
}

/* ------------- Connect && Send Functions ------------- */

bool Client::ConnServer()
{
	//  set server_addr info
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	server_addr.sin_port = htons(port);

	// connect to server
	if (::connect(server_socket, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		qDebug() << "Failed: Connect to Server";
		return false;
	}
	else {
		ui.tb_info->append("Success: Connect to Server");
	}

	// show default size of buffer
	int opt_val;
	int opt_len = sizeof(opt_val);
	if (getsockopt(server_socket, SOL_SOCKET, SO_RCVBUF, (char*)&opt_val, &opt_len) == 0) {
		QString tmp = "Default size of receive buffer: " + QString::number(opt_val) + "B";
		ui.tb_info->append(tmp);

		tmp.clear();
		getsockopt(server_socket, SOL_SOCKET, SO_SNDBUF, (char*)&opt_val, &opt_len);
		tmp = "Default size of send buffer: " + QString::number(opt_val) + "B";
		ui.tb_info->append(tmp);
	}
	
	isConn = true;
	ui.tb_content->setText("Message:\n");

	// start thread to recv data
	trs->start();

	return true;
}

bool Client::SendStr(SOCKET sock, QString in, QString & info, char buf[])
{
	if (in.isEmpty()) {
		info = "Input is empty.";
		return false;
	}

	int send_len = 0;					// length of Data to Buffer successfully
	string in_str = qstr2str(in);		// QString to string

	// data_len = sizeof(size_t) + len(in_str) + sizeof('\0')
	size_t data_len = sizeof(size_t) + in_str.length() + sizeof(char);

	str_to_arr(in_str, buf);					// transform data: str to char[]
	send_len = send(sock, buf, data_len, 0);	// send data

	info = "Data:" + QString::number(data_len) + "B, Send:" + QString::number(send_len) + "B";

	if (send_len == SOCKET_ERROR) {
		return false;
	}
	return true;
}

void Client::closeEvent(QCloseEvent * e)
{
	// inform server: Client CLOSED.
	char tmp[32];
	QString tmp_qs;
	SendStr(server_socket, "Client CLOSED.", tmp_qs, tmp);

	close();	// close
}

/* ------------- private slots ------------- */

void Client::onPBSendClicked()
{
	char send_buf[BUF_LEN];

	QString res, info;	// send result && info

	if (isConn) {		// connected to Server
		QString input = ui.e_input->text();
		if (SendStr(server_socket, input, info, send_buf)) {	// Successully Send
			res = "Success: Send to Server " + info;
			ui.tb_content->append("Client: " + input);
		}
		else {
			res = "Failed: Send to Server " + info;
		}
	}	
	else {				// disconnected to server
		res = "Failed: Could not connect to Server";
	}

	// update UI
	ui.e_input->clear();
	ui.tb_info->append(res);
}

void Client::onMenuConnSerClicked()
{
	// try connecting to server
	if (!isConn) {
		ConnDlg* dlg = new ConnDlg();
		dlg->setWindowModality(Qt::ApplicationModal);
		dlg->show();

		if (dlg->exec() == QDialog::Accepted)
		{
			if (!ConnServer()) {
				QMessageBox::warning(this, "Warning", "Failed to connect to Server!");
			}
			else {
				ui.tb_info->append("Success: Connected to Server ...\n");
			}
		}
	}
	else {
		QMessageBox::information(this, "Tips", "Already Connected.");
	}
}

void Client::onMenuConnHostClicked()
{
	ip = LOCAL_IP;
	port = LOCAL_PORT;
	// try connecting to localhost
	if (!isConn) {
		if (!ConnServer()) {
			QMessageBox::warning(this, "Warning", "Failed to connect to Host!");
		}
		else {
			ui.tb_info->append("Success: Connected to Host ...\n");
		}
	}
	else {
		QMessageBox::information(this, "Tips", "Already Connected.");
	}
}

void Client::updateRecvStatus(QString res, QString info, bool succ, bool sc)
{
	if (sc) {
		ui.tb_info->append("\nSERVER CLOSED.\n");
		QMessageBox::information(this, "Tip", "Server closed.");
		return;
	}
	if (succ) {
		ui.tb_info->append("Success: Recv from Server " + info);
		ui.tb_content->append("Server: " + res);
	}
	else {
		ui.tb_info->append("Failed: Recv from Server " + info);
	}
}
