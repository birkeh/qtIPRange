#include "cimportexceldialog.h"
#include "ui_cimportexceldialog.h"

cImportExcelDialog::cImportExcelDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cImportExcelDialog)
{
	ui->setupUi(this);
}

cImportExcelDialog::~cImportExcelDialog()
{
	delete ui;
}
