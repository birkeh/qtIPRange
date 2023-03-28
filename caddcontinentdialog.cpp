#include "caddcontinentdialog.h"
#include "ui_caddcitydialog.h"

#include <QPushButton>


cAddCityDialog::cAddCityDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cAddCityDialog)
{
	ui->setupUi(this);
}

cAddCityDialog::~cAddCityDialog()
{
	delete ui;
}

void cAddCityDialog::on_m_lpCity_textChanged(const QString &arg1)
{
	if(arg1.isEmpty())
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

QString cAddCityDialog::city()
{
	return(ui->m_lpCity->text());
}

qint32 cAddCityDialog::postalCode()
{
	return(ui->m_lpPostalCode->value());
}
