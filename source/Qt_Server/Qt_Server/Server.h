#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include "ui_Server.h"
#include "TConnClient.h"
#include "TRecvClient.h"

#define SERVER_PORT 1234

using namespace std;

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = Q_NULLPTR);
	~Server();

	// Init == InitWSA() + InitServerSock()
	void Init();
	bool InitWSA();
	bool InitServerSock();

	// close all connections
	void closeAllConn();

	// Send QString
	bool SendStr(SOCKET sock, QString in, QString& info, char buf[]);

private:
    Ui::ServerClass ui;
	TConnClient* tcc;		// Thread of waiting for connections
	TRecvClient* trc;		// Thread of recving data from clients

private slots:
	void updateConnStatus(QString s);
	void updateRecvStatus(QString r, QString i, QString c, bool s, bool cc);
	void onMenuShowInfoClicked();
	void onPBSendClicked();

};
