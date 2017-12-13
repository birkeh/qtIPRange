#include "cfinddialogipaddress.h"
#include "ui_cfinddialogipaddress.h"

#include <QPushButton>


cFindDialogIPAddress::cFindDialogIPAddress(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cFindDialogIPAddress)
{
	ui->setupUi(this);

	ui->m_lpIPAddress->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpIPRange->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpOriLocation->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpLocation->setEnabled(!ui->m_lpFindIn->isChecked());

	ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

cFindDialogIPAddress::~cFindDialogIPAddress()
{
	delete ui;
}

void cFindDialogIPAddress::on_m_lpFindIn_clicked()
{
	ui->m_lpIPAddress->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpIPRange->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpOriLocation->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpLocation->setEnabled(!ui->m_lpFindIn->isChecked());
}

void cFindDialogIPAddress::on_m_lpFindText_textChanged(const QString &arg1)
{
	if(ui->m_lpFindText->text().isEmpty())
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void cFindDialogIPAddress::setFindText(const QString& szFindText)
{
	ui->m_lpFindText->setText(szFindText);
	ui->m_lpFindText->selectAll();
}

QString cFindDialogIPAddress::findText()
{
	return(ui->m_lpFindText->text());
}

void cFindDialogIPAddress::setFindFlags(const quint16 &flags)
{
	if((flags & FindInAll) == FindInAll)
	{
		ui->m_lpFindIn->setChecked(true);
		on_m_lpFindIn_clicked();
		return;
	}

	ui->m_lpFindIn->setChecked(false);
	on_m_lpFindIn_clicked();

	ui->m_lpIPAddress->setChecked(flags & FindInIPAddress);
	ui->m_lpIPRange->setChecked(flags & FindInRange);
	ui->m_lpOriLocation->setChecked(flags & FindInOriLocation);
	ui->m_lpLocation->setChecked(flags & FindInLocation);
}

quint16 cFindDialogIPAddress::findFlags()
{
	quint16	flags	= 0;

	if(ui->m_lpFindIn->isChecked())
		return(FindInAll);

	if(ui->m_lpIPAddress->isChecked())
		flags	|= FindInIPAddress;
	if(ui->m_lpIPRange->isChecked())
		flags	|= FindInRange;
	if(ui->m_lpOriLocation->isChecked())
		flags	|= FindInOriLocation;
	if(ui->m_lpLocation->isChecked())
		flags	|= FindInLocation;

	return(flags);
}
