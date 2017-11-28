#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QFileDialog>
#include <QDir>

#include <QTime>
#include <QDebug>


cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow)
{
	ui->setupUi(this);

	m_lpIPRangeModel			= new QStandardItemModel(0, 1);
	ui->m_lpIPRangeList->setModel(m_lpIPRangeModel);
//	ui->m_lpIPRangeView->setItemDelegate(new cVideoViewItemDelegate());

	cIPRange	ipRange("10.0.0.1/24");

	QString		tmp;

	tmp	= ipRange.firstIPAddress();
	tmp	= ipRange.lastIPAddress();
	tmp	= ipRange.broadcastIPAddress();
	tmp	= ipRange.netmask();
	tmp	= ipRange.range();

	tmp	= "";
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

void cMainWindow::on_m_lpMenuFileOpen_triggered()
{
	QDir	dir;
	QString	strHome		= dir.homePath() + QDir::separator();
	QString	strFileName = QFileDialog::getOpenFileName(this, tr("Open IP Ranges"), strHome, tr("IP Ranges Files (*.xml)"));

	if(strFileName.isEmpty())
		return;

	cIPRangeList	ipRangeList;
	ipRangeList.load(strFileName);

	m_lpIPRangeModel->clear();
	QStringList	headerLabels	= QStringList() << tr("IP Range") << tr("Name") << tr("Location") << tr("Address") << tr("City");
	m_lpIPRangeModel->setHorizontalHeaderLabels(headerLabels);
	ipRangeList.fillList(m_lpIPRangeModel);

	ui->m_lpIPRangeList->expandAll();
	ui->m_lpIPRangeList->resizeColumnToContents(0);
	ui->m_lpIPRangeList->resizeColumnToContents(1);
	ui->m_lpIPRangeList->resizeColumnToContents(2);
	ui->m_lpIPRangeList->resizeColumnToContents(3);
	ui->m_lpIPRangeList->resizeColumnToContents(4);
}

void cMainWindow::on_m_lpMenuFileSave_triggered()
{
}

void cMainWindow::on_m_lpMenuFileSaveAs_triggered()
{
}
