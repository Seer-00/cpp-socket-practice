#pragma once

#include <QtWidgets/QMainWindow>
#include "ConnDlg.h"
#include "TRecvServer.h"
#include "ui_Client.h"

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = Q_NULLPTR);
	~Client();

	// Init() = InitWSA() + create server_socket
	void Init();
	bool InitWSA();

	// connect to server
	bool ConnServer();

	// send QString
	bool SendStr(SOCKET sock, QString in, QString& info, char buf[]);

	// close event
	void closeEvent(QCloseEvent * e);

private:
    Ui::ClientClass ui;
	TRecvServer* trs;

private slots:
	void onPBSendClicked();
	void onMenuConnSerClicked();
	void onMenuConnHostClicked();
	void updateRecvStatus(QString res, QString info, bool succ, bool sc);

};



