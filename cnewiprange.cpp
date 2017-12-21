#include "cnewiprange.h"
#include "ui_cnewiprange.h"


cNewIPRange::cNewIPRange(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cNewIPRange)
{
	ui->setupUi(this);
}

cNewIPRange::~cNewIPRange()
{
	delete ui;
}

void cNewIPRange::setIPRange(const QString &szIPRange)
{
	ui->m_lpIPRange->setText(szIPRange);
}

QString cNewIPRange::ipRange()
{
	return(ui->m_lpIPRange->text());
}

void cNewIPRange::setLocation(const cLocationList& locationList, const QString &szLocation)
{
	for(int x = 0;x < locationList.count();x++)
	{
		cLocation*	lpLocation	= locationList.at(x);
		ui->m_lpLocation->addItem(locationList.at(x)->name(), locationList.at(x)->locationID());
	}

	if(szLocation.isEmpty())
		return;

	ui->m_lpLocation->setCurrentText(szLocation);
}

qint32 cNewIPRange::location()
{
	return(ui->m_lpLocation->currentData().toInt());
}
