#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QFileDialog>
#include <QDir>

#include <QTime>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>

#include <QFile>
#include <QTextStream>


cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow)
{
	ui->setupUi(this);

	m_lpIPRangeModel			= new QStandardItemModel(0, 1);
	ui->m_lpIPRangeList->setModel(m_lpIPRangeModel);
	ui->m_lpIPRangeList->setItemDelegate(new cIPRangeItemDelegate());

	m_db	= QSqlDatabase::addDatabase("QSQLITE");
	m_db.setHostName("localhost");
	m_db.setDatabaseName(QString("%1%2qtIPRange.sqlite").arg(QDir::homePath()).arg(QDir::separator()));
	if(!m_db.open())
		return;

	QSqlQuery	query;

	if(!m_db.tables().contains("country"))
	{
		query.exec("CREATE TABLE country ("
					"id INTEGER PRIMARY KEY, "
					"name TEXT"
				   ")");
	}

	if(!m_db.tables().contains("federal_state"))
	{
		query.exec("CREATE TABLE federal_state ( "
				   "id INTEGER PRIMARY KEY, "
				   "name TEXT)");
	}

	if(!m_db.tables().contains("city"))
	{
		query.exec("CREATE TABLE city ( "
				   "id INTEGER PRIMARY KEY, "
				   "name TEXT, "
				   "postal_code INTEGER)");
	}

	if(!m_db.tables().contains("location"))
	{
		query.exec("CREATE TABLE location ( "
				   "id INTEGER PRIMARY KEY, "
				   "name TEXT, "
				   "location TEXT, "
				   "alternate_location TEXT, "
				   "address TEXT, "
				   "country_id INTEGER, "
				   "federal_state_id INTEGER, "
				   "city_id INTEGER)");
	}

	if(!m_db.tables().contains("ip_range_location"))
	{
		query.exec("CREATE TABLE ip_range_location ( "
				   "name TEXT, "
				   "location_id INTEGER, "
				   "subnet INTEGER, "
				   "prefix INTEGER, "
				   "subnet1 INTEGER, "
				   "subnet2 INTEGER, "
				   "subnet3 INTEGER, "
				   "mask INTEGER, "
				   "iplow INTEGER, "
				   "iphigh INTEGER)");
	}

	loadLocationList();
	loadIPRangeList();
	displayIPRangeList();
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

void cMainWindow::on_m_lpMenuFileOpen_triggered()
{
	QFile file("c:\\temp\\out.txt");
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	for(int x = 0;x < m_ipRangeList.count();x++)
	{
		cIPRange*	lpRange		= m_ipRangeList.at(x);
		cLocation*	lpLocation	= m_locationList.find(lpRange->location());

		out << lpRange->name();
		out << "\t";
		out << lpLocation->name();
		out << "\n";
	}

	file.close();

/*
	m_ipRangeList.add("1.46.103.0/25");
	m_ipRangeList.sort();
	m_ipRangeList.verify();
	displayIPRangeList();
*/
/*
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
*/
}

void cMainWindow::on_m_lpMenuFileSave_triggered()
{
}

void cMainWindow::on_m_lpMenuFileSaveAs_triggered()
{
}

void cMainWindow::loadLocationList()
{
	QSqlQuery	query;

	query.exec("SELECT id, name, location, alternate_location, address, country_id, federal_state_id, city_id FROM location ORDER BY name");
	while(query.next())
	{
		cLocation*	lpLocation	= m_locationList.add(query.value("id").toInt());
		lpLocation->setName(query.value("name").toString());
		lpLocation->setLocation(query.value("location").toString());
		lpLocation->setAlternateLocation(query.value("alternate_location").toString());
		lpLocation->setAddress(query.value("address").toString());
		lpLocation->setCountryID(query.value("country_id").toInt());
		lpLocation->setFederalStateID(query.value("federal_state_id").toInt());
		lpLocation->setCityID(query.value("city_id").toInt());
	}
}

void cMainWindow::loadIPRangeList()
{
	QSqlQuery	query;

	query.exec("SELECT name, location_id FROM ip_range_location");
	while(query.next())
	{
		cIPRange*	lpIPRange	= m_ipRangeList.add(query.value("name").toString());
		lpIPRange->setLocation(query.value("location_id").toInt());
	}

	m_ipRangeList.sort();
	m_ipRangeList.verify();
}

void cMainWindow::displayIPRangeList()
{
	m_lpIPRangeModel->clear();

	QStringList	header;
	header << "Name" << "Range" << "Base IP" << "Prefix" << "Subnet Mask" << "Broadcast IP" << "First IP" << "Last IP" << "Location";

	m_lpIPRangeModel->setHorizontalHeaderLabels(header);

	for(int x = 0;x < m_ipRangeList.count();x++)
	{
		QList<QStandardItem*>	lpItems;

		for(int z = 0;z < header.count();z++)
			lpItems.append(new QStandardItem);

		cIPRange*		lpRange		= m_ipRangeList.at(x);
		cLocation*		lpLocation	= m_locationList.find(lpRange->location());

		lpItems.at(0)->setText(lpRange->name());
		lpItems.at(1)->setText(lpRange->range());
		lpItems.at(2)->setText(lpRange->IPAddress().IPAddress());
		lpItems.at(3)->setText(QString("%1").arg(lpRange->prefix()));
		lpItems.at(4)->setText(lpRange->netmask());
		lpItems.at(5)->setText(lpRange->broadcastIPAddress());
		lpItems.at(6)->setText(lpRange->firstIPAddress());
		lpItems.at(7)->setText(lpRange->lastIPAddress());
		if(lpLocation)
			lpItems.at(8)->setText(lpLocation->name());

		lpItems.at(0)->setData(QVariant::fromValue(lpRange), Qt::UserRole);
		lpItems.at(8)->setData(QVariant::fromValue(&m_locationList), Qt::UserRole);

		if(!lpRange->ok())
			lpItems.at(0)->setBackground(Qt::red);

		m_lpIPRangeModel->appendRow(lpItems);
	}

	for(int z = 0;z < header.count();z++)
		ui->m_lpIPRangeList->resizeColumnToContents(z);
}
