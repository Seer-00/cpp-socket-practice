#include "Server.h"
#include "ServerInfo.h"
#pragma comment(lib, "ws2_32.lib")

SOCKET server_socket;
SOCKADDR_IN server_addr;

/* ------------- Constructor && Destructor ------------- */

Server::Server(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowTitle("Server");

	ui.e_input->setFocus();		// set focus on inputline
	// set shortcut: Enter == (pushBtn) Send
	connect(ui.e_input, SIGNAL(returnPressed()), ui.pb_send, 
		SIGNAL(clicked()), Qt::UniqueConnection);

	// set font
	// QFont font("ZYSong18030", 12);
	QFont font("Microsoft YaHei", 10);
	ui.tb_info->setFont(font);
	ui.tb_content->setFont(font);
	
	// set spin_box "sendto"
	ui.sp_sendto->setPrefix("client ");
	ui.sp_sendto->setMinimum(1);
	
	// init
	Init();

	// Thread: connect client
	tcc = new TConnClient();
	connect(tcc, SIGNAL(sig_conn_res(QString)), this, SLOT(updateConnStatus(QString)));
	tcc->start();

	// Thread: receive from client
	trc = new TRecvClient();
	connect(trc, SIGNAL(sig_recv_res(QString, QString, QString, bool, bool)),
		this, SLOT(updateRecvStatus(QString, QString, QString, bool, bool)));
	trc->start();
}

Server::~Server()
{
	tcc->quit();		// stop thread connect to clients
	trc->quit();		// stop thread receive clients

	closeAllConn();		// clear client list
	Sleep(1000);		// sleep for sending "Server CLOSED."

	closesocket(server_socket);
	WSACleanup();
}

/* ------------- private slots ------------- */

void Server::updateConnStatus(QString s)
{
	ui.tb_info->append(s);	// update info
	int origin_num = (ui.lb_c_num->text()).toInt();
	ui.lb_c_num->setText(QString::number(origin_num + 1));

	if (!(ui.tb_content->toPlainText()).startsWith("Message:\n")) {
		ui.tb_content->setText("Message:\n");
	}
}

void Server::updateRecvStatus(QString res, QString info, QString cid, bool succ, bool cclo)
{
	if (cclo) {		// client closed
		// show info
		QMessageBox::information(this, "Tip", "Client[" + cid + "] Closed.");
		
		// update UI
		ui.tb_info->append("EXIT: Client[" + cid + "] ");
		int origin_num = (ui.lb_c_num->text()).toInt();
		ui.lb_c_num->setText(QString::number(origin_num - 1));

		// Remove client obj from l_client
		for (list<Client *>::iterator it = l_client.begin(); it != l_client.end(); ++it) {
			if ((*it)->cid == cid.toUShort()) {
				delete *it;
				l_client.erase(it);
				break;
			}
		}
		qDebug() << "Client closed:" << QString::number(l_client.size());
		return;
	}
	if (succ) {
		ui.tb_info->append("Success: Recv from Client[" + cid + "] " + info);
		ui.tb_content->append("Client[" + cid + "] -> Server: " + res);
	}
	else {
		ui.tb_info->append("Failed: Recv from Client[" + cid + "] " + info);
	}
}

void Server::onMenuShowInfoClicked()
{
	ServerInfo* dlg = new ServerInfo(this);
	dlg->show();
	dlg->setAttribute(Qt::WA_DeleteOnClose, true);
}

void Server::onPBSendClicked()
{
	char send_buf[BUF_LEN];

	QString res, info;						// send result && info
	u_short cid = ui.sp_sendto->value();	// get target client ID
	QString qs_cid = QString::number(cid);	
	Client* pClient = Client::findClientByCID(cid);	// get pointer to client obj
	
	if (pClient != nullptr) {				// maybe accessible
		QString input = ui.e_input->text();
		if (SendStr(pClient->accept_socket, input, info, send_buf)) {	// Success
			res = "Success: Send to Client[" + qs_cid + "] " + info;
			ui.tb_content->append("Server -> Client[" + qs_cid + "]: " + input);
		}
		else {
			res = "Failed: Send to Client[" + qs_cid + "] " + info;
		}
	}
	else {									
		res = "Failed: Could not connect to Client[" + qs_cid + "]";
	}

	// update UI
	ui.e_input->clear();					
	ui.tb_info->append(res);				
}

/* ------------- Initialize && Close Functions ------------- */

void Server::Init()
{
	// init WSA
	if (!InitWSA()) {
		QMessageBox::warning(this, "Warning", "Initialize WSA Failed.");
		exit(-1);
	}

	// definition of server_socket must be set AFTER "initWSA()"
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (!InitServerSock()) {
		QMessageBox::warning(this, "Warning", "Initialize server socket Failed.");
		exit(-1);
	}
	else {
		ui.tb_info->append("Server is waiting for connections ...\n");
	}
}

bool Server::InitWSA()
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

bool Server::InitServerSock()
{
	//  set server ADDR info
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	// bind: socket and SOCKADDR
	if (::bind(server_socket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		qDebug() << "Failed: Bind socket";
		WSACleanup();
		return false;
	}
	else {
		ui.tb_info->append("Success: Bind socket");
	}

	// set: socket listening state
	if (listen(server_socket, SOMAXCONN) < 0) {
		qDebug() << "Failed: Set listening state";
		WSACleanup();
		return false;
	}
	else {
		ui.tb_info->append("Success: Set listening state");
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

	return true;
}

void Server::closeAllConn()
{
	char tmp_buf[32];
	QString tmp_qs;
	for (list<Client*>::iterator i = l_client.begin(); i != l_client.end();) {
		SendStr((*i)->accept_socket, "Server CLOSED.", tmp_qs, tmp_buf);
		delete *i;
		i = l_client.erase(i);
	}
}

/* ------------- Send Function ------------- */

bool Server::SendStr(SOCKET sock, QString in, QString& info, char buf[])
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


