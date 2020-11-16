#include <QtWidgets>
#include "ConnDlg.h"

extern string ip;
extern int port;

ConnDlg::ConnDlg(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);

	ui.pb_ok->setFocus();
	// set shortcut: Enter == (pushBtn) OK
	connect(ui.e_port, SIGNAL(returnPressed()), ui.pb_ok,
		SIGNAL(clicked()), Qt::UniqueConnection);

}

ConnDlg::~ConnDlg() { }

void ConnDlg::onPBOkCLicked()
{
	QString ip_qs = ui.e_ip->text();
	QString port_qs = ui.e_port->text();

	ip = qstr2str(ip_qs);
	port = port_qs.toInt();

	accept();
}

void ConnDlg::onPBClrClicked()
{
	ui.e_ip->clear();
	ui.e_port->clear();
}
