#include <QtWidgets>
#include "Server.h"
#include "ServerInfo.h"

ServerInfo::ServerInfo(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Server Information");
	showServerInfo();
}

void ServerInfo::showServerInfo()
{
	char name[256];
	int getNameRet = gethostname(name, sizeof(name));
	hostent *host = gethostbyname(name);

	if (NULL == host) {
		return;
	}

	in_addr *pAddr = (in_addr*)*host->h_addr_list;

	for (int i = 0; i < (strlen((char*)*host->h_addr_list)
		- strlen(host->h_name)) / 4 && pAddr; i++) {
		QString addr = inet_ntoa(pAddr[i]);
		ui.info->append("Host IP Address[" + QString::number(i) + "]: " + addr);
	}

	ui.info->append("\nApplication Port: " + QString::number(SERVER_PORT));
}

