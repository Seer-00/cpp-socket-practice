#pragma once

#include <QDialog>
#include "ui_ServerInfo.h"

class ServerInfo : public QDialog, public Ui::InfoDlg
{
	Q_OBJECT

public:
	ServerInfo(QWidget* parent = Q_NULLPTR);
	void showServerInfo();

private:
	Ui::InfoDlg ui;
};

