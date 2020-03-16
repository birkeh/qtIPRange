#ifndef CIMPORTEXCELDIALOG_H
#define CIMPORTEXCELDIALOG_H

#include <QDialog>

namespace Ui {
class cImportExcelDialog;
}

class cImportExcelDialog : public QDialog
{
	Q_OBJECT

public:
	explicit cImportExcelDialog(QWidget *parent = nullptr);
	~cImportExcelDialog();

private:
	Ui::cImportExcelDialog *ui;
};

#endif // CIMPORTEXCELDIALOG_H
