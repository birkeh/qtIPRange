#include "cfinddialogiprange.h"
#include "ui_cfinddialogiprange.h"

#include <QPushButton>


cFindDialogIPRange::cFindDialogIPRange(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cFindDialogIPRange)
{
	ui->setupUi(this);

	ui->m_lpName->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpRange->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpBaseIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpPrefix->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpSubnetMask->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpBroadcastIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpFirstIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpLastIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpLocation->setEnabled(!ui->m_lpFindIn->isChecked());

	ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

cFindDialogIPRange::~cFindDialogIPRange()
{
	delete ui;
}

void cFindDialogIPRange::on_m_lpFindIn_clicked()
{
	ui->m_lpName->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpRange->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpBaseIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpPrefix->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpSubnetMask->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpBroadcastIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpFirstIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpLastIP->setEnabled(!ui->m_lpFindIn->isChecked());
	ui->m_lpLocation->setEnabled(!ui->m_lpFindIn->isChecked());
}

void cFindDialogIPRange::on_m_lpFindText_textChanged(const QString &/*arg1*/)
{
	if(ui->m_lpFindText->text().isEmpty())
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void cFindDialogIPRange::setFindText(const QString& szFindText)
{
	ui->m_lpFindText->setText(szFindText);
	ui->m_lpFindText->selectAll();
}

QString cFindDialogIPRange::findText()
{
	return(ui->m_lpFindText->text());
}

void cFindDialogIPRange::setFindFlags(const quint16 &flags)
{
	if((flags & FindInAll) == FindInAll)
	{
		ui->m_lpFindIn->setChecked(true);
		on_m_lpFindIn_clicked();
		return;
	}

	ui->m_lpFindIn->setChecked(false);
	on_m_lpFindIn_clicked();

	ui->m_lpName->setChecked(flags & FindInName);
	ui->m_lpRange->setChecked(flags & FindInRange);
	ui->m_lpBaseIP->setChecked(flags & FindInBaseIP);
	ui->m_lpPrefix->setChecked(flags & FindInPrefix);
	ui->m_lpSubnetMask->setChecked(flags & FindInSubnetMask);
	ui->m_lpBroadcastIP->setChecked(flags & FindInBroadcastIP);
	ui->m_lpFirstIP->setChecked(flags & FindInFirstIP);
	ui->m_lpLastIP->setChecked(flags & FindInLastIP);
	ui->m_lpLocation->setChecked(flags & FindInLocation);
}

quint16 cFindDialogIPRange::findFlags()
{
	quint16	flags	= 0;

	if(ui->m_lpFindIn->isChecked())
		return(FindInAll);

	if(ui->m_lpName->isChecked())
		flags	|= FindInName;
	if(ui->m_lpRange->isChecked())
		flags	|= FindInRange;
	if(ui->m_lpBaseIP->isChecked())
		flags	|= FindInBaseIP;
	if(ui->m_lpPrefix->isChecked())
		flags	|= FindInPrefix;
	if(ui->m_lpSubnetMask->isChecked())
		flags	|= FindInSubnetMask;
	if(ui->m_lpBroadcastIP->isChecked())
		flags	|= FindInBroadcastIP;
	if(ui->m_lpFirstIP->isChecked())
		flags	|= FindInFirstIP;
	if(ui->m_lpLastIP->isChecked())
		flags	|= FindInLastIP;
	if(ui->m_lpLocation->isChecked())
		flags	|= FindInLocation;

	return(flags);
}
