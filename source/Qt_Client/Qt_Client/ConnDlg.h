#ifndef LOGINDLG_H
#define LOGINDLG_H
#include <QDialog>
#include "Supplement.h"
#include "ui_ConnDlg.h"
class ConnDlg : public QDialog, public Ui::ConnDlg
{
	Q_OBJECT

public:
	ConnDlg(QWidget* parent = Q_NULLPTR);
	~ConnDlg();

private:
	Ui::ConnDlg ui;

private slots:
	void onPBOkCLicked();
	void onPBClrClicked();

};

#endif // LOGINDLG_H
