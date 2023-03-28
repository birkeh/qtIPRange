#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cnewiprange.h"

#include "caddcitydialog.h"

#include "xlsxdocument.h"

#include "cimportexceldialog.h"

#include <QFileDialog>
#include <QDir>

#include <QTime>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSettings>

#include <QElapsedTimer>

#include <QMessageBox>

#include <QCloseEvent>


#define REPORTSERVER	"svuitpburedb01.tsa.prov-uniqa.net"
#define REPORTDB		"reporting"
#define	REPORTUSER		"reporting"
#define REPORTPASS		"Kellerassel-1"


cMainWindow::cMainWindow(cSplashScreen* lpSplashScreen, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow),
	m_szIPRangeFindText(""),
	m_IPRangeFindFlags(cFindDialogIPRange::FindInAll),
	m_szIPAddressFindText(""),
	m_IPAddressFindFlags(cFindDialogIPAddress::FindInAll)
{
	ui->setupUi(this);

	QSettings	settings;
	m_szHostName		= settings.value("database/hostName", REPORTSERVER).toString();
	m_szDatabaseName	= settings.value("database/databaseName", REPORTDB).toString();
	m_szUserName		= settings.value("database/userName", REPORTUSER).toString();
	m_szPassword		= settings.value("database/password", REPORTPASS).toString();

	ui->m_lpTab->setCurrentIndex(0);
	ui->m_lpMenuActionFindPreviousError->setEnabled(false);
	ui->m_lpMenuActionFindNextError->setEnabled(false);

	m_lpProgressBar				= new QProgressBar(this);
	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->addPermanentWidget(m_lpProgressBar);

	m_lpIPRangeModel			= new QStandardItemModel(0, 1);
	ui->m_lpIPRangeList->setModel(m_lpIPRangeModel);
	ui->m_lpIPRangeList->setItemDelegate(new cIPRangeItemDelegate());

	m_lpIPAddressModel			= new QStandardItemModel(0, 1);
	ui->m_lpIPAddressList->setModel(m_lpIPAddressModel);

	connect((cIPRangeItemDelegate*)ui->m_lpIPRangeList->itemDelegate(), SIGNAL(ipRangeChanged(cIPRange*,QStandardItem*)), this, SLOT(ipRangeChanged(cIPRange*,QStandardItem*)));
	connect((cIPRangeItemDelegate*)ui->m_lpIPRangeList->itemDelegate(), SIGNAL(locationChanged(cIPRange*,QStandardItem*)), this, SLOT(locationChanged(cIPRange*,QStandardItem*)));

	m_db	= QSqlDatabase::addDatabase("QSQLITE");
	m_db.setHostName("localhost");
	m_db.setDatabaseName(QString("%1%2qtIPRange.sqlite").arg(QDir::homePath()).arg(QDir::separator()));
	if(!m_db.open())
		return;

	QSqlQuery	query(m_db);

	if(!m_db.tables().contains("ip_range_location"))
	{
		query.exec("CREATE TABLE ip_range_location ( "
				   "name TEXT, "
				   "cityID INTEGER, "
				   "subnet INTEGER, "
				   "prefix INTEGER, "
				   "subnet1 INTEGER, "
				   "subnet2 INTEGER, "
				   "subnet3 INTEGER, "
				   "mask INTEGER, "
				   "iplow INTEGER, "
				   "iphigh INTEGER)");
	}

	if(!m_db.tables().contains("continent"))
	{
		query.exec("CREATE TABLE continent ( "
				   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "name STRING)");
	}

	if(!m_db.tables().contains("country"))
	{
		query.exec("CREATE TABLE continent ( "
				   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "name STRING, "
				   "continentID INTEGER, "
				   "CONSTRAINT country_continentID_FK FOREIGN KEY (continentID) REFERENCES continent(id))");
	}

	if(!m_db.tables().contains("region"))
	{
		query.exec("CREATE TABLE continent ( "
				   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "name STRING, "
				   "countryID INTEGER, "
				   "CONSTRAINT region_countryID_FK FOREIGN KEY (countryID) REFERENCES country(id))");
	}

	if(!m_db.tables().contains("city"))
	{
		query.exec("CREATE TABLE continent ( "
				   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "name STRING, "
				   "regionID INTEGER, "
				   "CONSTRAINT city_regionID_FK FOREIGN KEY (regionID) REFERENCES region(id))");
	}

	ui->m_lpFilterError->setCheckState(Qt::PartiallyChecked);
	ui->m_lpFilterOldLocation->setCheckState(Qt::PartiallyChecked);

	loadLocationList(lpSplashScreen);

	lpSplashScreen->showStatusMessage(QObject::tr("<center><big><big>loading ranges...</big></big></denter>"));
	qApp->processEvents();
	loadIPRangeList();
	displayIPRangeList();
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

void cMainWindow::closeEvent(QCloseEvent *event)
{
	QSettings	settings;
	settings.setValue("main/width", QVariant::fromValue(size().width()));
	settings.setValue("main/height", QVariant::fromValue(size().height()));
	settings.setValue("main/x", QVariant::fromValue(x()));
	settings.setValue("main/y", QVariant::fromValue(y()));
	if(this->isMaximized())
		settings.setValue("main/maximized", QVariant::fromValue(true));
	else
		settings.setValue("main/maximized", QVariant::fromValue(false));

	event->accept();
}

void cMainWindow::on_m_lpMenuFileOpen_triggered()
{
	m_ipAddressList.clear();

	QDir	dir;
	QString	strHome		= dir.homePath() + QDir::separator();
	QString	strFileName = QFileDialog::getOpenFileName(this, tr("Open IP Addresses"), strHome, tr("IP Addresses Files (*.sql)"));

	if(strFileName.isEmpty())
		return;

	QFile	file(strFileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream	in(&file);

	while(!in.atEnd())
	{
		QString line = in.readLine();

		if(line.contains("VALUES"))
		{
			QString		str		= line.mid(line.indexOf("VALUES")+7);
			str					= str.left(str.length()-1);
			QStringList	ipList	= str.split(",");

			ipList.removeDuplicates();
			for(int x = 0;x < ipList.count();x++)
			{
				QString	szIP	= ipList.at(x);
				szIP			= szIP.mid(2, szIP.length()-4);
				m_ipAddressList.add(szIP);
			}
		}
	}

	file.close();

	m_ipAddressList.sort();
	displayIPAddressList();
//	verifyIPAddressList();
}

void cMainWindow::on_m_lpMenuFileSave_triggered()
{
}

void cMainWindow::on_m_lpMenuFileSaveAs_triggered()
{
}

void cMainWindow::loadLocationList(cSplashScreen* lpSplashScreen)
{
	if(lpSplashScreen)
	{
		lpSplashScreen->showStatusMessage(QObject::tr("<center><big><big>connecting to database...</big></big></denter>"));
		qApp->processEvents();
	}

	m_locationList.clear();

	if(lpSplashScreen)
	{
		lpSplashScreen->showStatusMessage(QObject::tr("<center><big><big>loading locations...</big></big></denter>"));
		qApp->processEvents();
	}

	QSqlQuery	query;

	query.exec("SELECT		l.id id, "
			   "			l.name name, "
			   "			l.regionID, "
			   "			l.location location, "
			   "			l.alternate_location alternate_location, "
			   "			l.address address, "
			   "			c.name country, "
			   "			f.name federal_state, "
			   "			ci.name city, "
			   "			ci.postal_code "
			   "FROM		city ci "
			   "LEFT JOIN	region r ON (ci.regionID = r.id) "
			   "LEFT JOIN	country c ON (r.countryID = c.id) "
			   "LEFT JOIN	continent co ON (c.continentID = co.id) "
			   "ORDER BY 	name");
	while(query.next())
	{
		cLocation*	lpLocation	= m_locationList.add(query.value("id").toInt());
		lpLocation->setName(query.value("name").toString());
		lpLocation->setLocation(query.value("location").toString());
		lpLocation->setAlternateLocation(query.value("alternate_location").toString());
		lpLocation->setAddress(query.value("address").toString());
		lpLocation->setCountry(query.value("country").toString());
		lpLocation->setFederalState(query.value("federal_state").toString());
		lpLocation->setCity(query.value("city").toString());
		lpLocation->setPostalCode(query.value("postal_code").toInt());
	}
}

void cMainWindow::loadIPRangeList()
{
	m_ipRangeList.clear();

	QSqlQuery	query(m_db);

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
		{
			lpItems.at(8)->setText(lpLocation->name());

			QString	szHint	= QString("Name: %1").arg(lpLocation->name());

			if(!lpLocation->location().isEmpty())
				szHint.append(QString("\nLocation: %1").arg(lpLocation->location()));

			if(!lpLocation->alternateLocation().isEmpty())
				szHint.append(QString("\nAlternate Location: %1").arg(lpLocation->alternateLocation()));

			if(!lpLocation->address().isEmpty())
				szHint.append(QString("\nAddress: %1").arg(lpLocation->address()));

			if(!lpLocation->city().isEmpty())
				szHint.append(QString("\nCity: %1").arg(lpLocation->city()));

			if(lpLocation->postalCode() > 999)
				szHint.append(QString("\nPostal Code: %1").arg(lpLocation->postalCode()));

			if(!lpLocation->federalState().isEmpty())
				szHint.append(QString("\nFederal State: %1").arg(lpLocation->federalState()));

			if(!lpLocation->country().isEmpty())
				szHint.append(QString("\nCountry: %1").arg(lpLocation->country()));

			lpItems.at(8)->setToolTip(szHint);
		}

		lpItems.at(0)->setData(QVariant::fromValue(lpRange), Qt::UserRole);
		lpItems.at(8)->setData(QVariant::fromValue(&m_locationList), Qt::UserRole);

		if(!lpRange->ok())
			lpItems.at(0)->setBackground(Qt::red);

		m_lpIPRangeModel->appendRow(lpItems);
	}

	for(int z = 0;z < header.count();z++)
		ui->m_lpIPRangeList->resizeColumnToContents(z);
}

void cMainWindow::displayIPAddressList()
{
	m_lpIPAddressModel->clear();

	qint16		cnt			= 0;
	qint16		max			= m_ipAddressList.count()/200;

	QStringList	header;
	header << "IP Address" << "Range" << "original Location" << "Location";

	m_lpIPAddressModel->setHorizontalHeaderLabels(header);

	m_lpProgressBar->setVisible(true);
	m_lpProgressBar->setMinimum(0);
	m_lpProgressBar->setMaximum(m_ipAddressList.count());

	ui->m_lpStatusBar->showMessage("loading IP address list ...");
	for(int x = 0;x < m_ipAddressList.count();x++)
	{
		QList<QStandardItem*>	lpItems;

		for(int z = 0;z < header.count();z++)
			lpItems.append(new QStandardItem);

		cIPAddress*		lpIPAddress	= m_ipAddressList.at(x);
		cIPRange*		lpIPRange	= m_ipRangeList.findRange(lpIPAddress->IPAddressBin());
		cLocation*		lpLocation	= 0;

		lpIPAddress->setIPRange(lpIPRange);

		if(lpIPRange)
			lpLocation	= m_locationList.find(lpIPRange->location());

		lpItems.at(0)->setText(lpIPAddress->IPAddress());
		if(lpIPRange)
			lpItems.at(1)->setText(lpIPRange->range());
		lpItems.at(2)->setText(lpIPAddress->address());
		if(lpLocation)
		{
			lpItems.at(3)->setText(lpLocation->name());

			QString	szHint	= QString("Name: %1").arg(lpLocation->name());

			if(!lpLocation->location().isEmpty())
				szHint.append(QString("\nLocation: %1").arg(lpLocation->location()));

			if(!lpLocation->alternateLocation().isEmpty())
				szHint.append(QString("\nAlternate Location: %1").arg(lpLocation->alternateLocation()));

			if(!lpLocation->address().isEmpty())
				szHint.append(QString("\nAddress: %1").arg(lpLocation->address()));

			if(!lpLocation->city().isEmpty())
				szHint.append(QString("\nCity: %1").arg(lpLocation->city()));

			if(lpLocation->postalCode() > 999)
				szHint.append(QString("\nPostal Code: %1").arg(lpLocation->postalCode()));

			if(!lpLocation->federalState().isEmpty())
				szHint.append(QString("\nFederal State: %1").arg(lpLocation->federalState()));

			if(!lpLocation->country().isEmpty())
				szHint.append(QString("\nCountry: %1").arg(lpLocation->country()));

			lpItems.at(3)->setToolTip(szHint);
		}

		lpItems.at(0)->setData(QVariant::fromValue(lpIPAddress), Qt::UserRole);

		if(!lpIPRange)
			lpItems.at(0)->setBackground(Qt::red);

		m_lpIPAddressModel->appendRow(lpItems);

		cnt++;
		if(cnt > max)
		{
			cnt = 0;
			m_lpProgressBar->setValue(x);
		}
	}

	for(int z = 0;z < header.count();z++)
		ui->m_lpIPAddressList->resizeColumnToContents(z);

	m_lpProgressBar->setVisible(false);

	ui->m_lpStatusBar->clearMessage();
}

void cMainWindow::verifyIPAddressList()
{
	filterError();
}

void cMainWindow::ipRangeChanged(cIPRange *lpIPRange, QStandardItem *lpItem)
{
	QList<QStandardItem*>	lpItems;
	QModelIndex				index	= lpItem->index();
	QStandardItemModel*		lpModel	= (QStandardItemModel*)index.model();

	for(int x = 0;x < 9;x++)
		lpItems.append(lpModel->itemFromIndex(lpModel->index(index.row(), x)));

	lpItems.at(0)->setText(lpIPRange->name());
	lpItems.at(1)->setText(lpIPRange->range());
	lpItems.at(2)->setText(lpIPRange->IPAddress().IPAddress());
	lpItems.at(3)->setText(QString("%1").arg(lpIPRange->prefix()));
	lpItems.at(4)->setText(lpIPRange->netmask());
	lpItems.at(5)->setText(lpIPRange->broadcastIPAddress());
	lpItems.at(6)->setText(lpIPRange->firstIPAddress());
	lpItems.at(7)->setText(lpIPRange->lastIPAddress());

	m_ipRangeList.sort();
	m_ipRangeList.verify();

	qint32					iPos	= m_ipRangeList.position(lpIPRange);
	qint32					iOldPos	= index.row();

	if(iPos >= 0)
	{
		QList<QStandardItem*>	itemList	= m_lpIPRangeModel->takeRow(index.row());
		m_lpIPRangeModel->insertRow(iPos, itemList);
		ui->m_lpIPRangeList->setCurrentIndex(lpModel->index(iPos, 0));
	}

	QBrush	brush	= m_lpIPRangeModel->item(0, 1)->background();

	qint32	iMin;
	qint32	iMax;

	iMin	= (iPos > 0) ? iPos-1 : 0;
	iMax	= (iPos < (m_lpIPRangeModel->rowCount()-1)) ? iPos + 2 : m_lpIPRangeModel->rowCount();
	for(int x = iMin;x < iMax;x++)
	{
		QStandardItem*	lpItem	= m_lpIPRangeModel->item(x, 0);
		cIPRange*		lpRange	= qvariant_cast<cIPRange*>(lpItem->data(Qt::UserRole));
		if(lpRange && lpRange->ok())
			lpItem->setBackground(brush);
		else
			lpItem->setBackground(Qt::red);
	}

	iMin	= (iOldPos > 0) ? iOldPos-1 : 0;
	iMax	= (iOldPos < (m_lpIPRangeModel->rowCount()-1)) ? iOldPos + 2 : m_lpIPRangeModel->rowCount();
	for(int x = iMin;x < iMax;x++)
	{
		QStandardItem*	lpItem	= m_lpIPRangeModel->item(x, 0);
		cIPRange*		lpRange	= qvariant_cast<cIPRange*>(lpItem->data(Qt::UserRole));
		if(lpRange && lpRange->ok())
			lpItem->setBackground(brush);
		else
			lpItem->setBackground(Qt::red);
	}

	saveRange(lpIPRange);

	filterError();
}

void cMainWindow::locationChanged(cIPRange* lpIPRange, QStandardItem* /*lpItem*/)
{
	saveRange(lpIPRange);
}

void cMainWindow::saveRange(cIPRange* lpIPRange)
{
	QString		strSQL;
	QSqlQuery	query(m_db);

	QString		szName		= lpIPRange->name();
	qint32		iLocation	= lpIPRange->location();
	qint32		iRange		= cIPAddress::ip2bin(lpIPRange->range());
	qint32		iPrefix		= lpIPRange->prefix();
	qint64		iSubnet1	= lpIPRange->subnet1BIN();
	qint64		iSubnet2	= lpIPRange->subnet2BIN();
	qint64		iSubnet3	= lpIPRange->subnet3BIN();
	quint64		iNetmask	= lpIPRange->netmaskBin();
	qint64		iFirstIP	= lpIPRange->firstIPAddressBin();
	qint64		iLastIP		= lpIPRange->lastIPAddressBin();
	QString		szOldName	= lpIPRange->oldName();

	strSQL	= QString("UPDATE ip_range_location SET name='%1', location_id=%2, subnet=%3, prefix=%4, subnet1=%5, subnet2=%6, subnet3=%7, mask=%8, iplow=%9, iphigh=%10 WHERE name='%11';")
			.arg(szName)
			.arg(iLocation)
			.arg(iRange)
			.arg(iPrefix)
			.arg(iSubnet1)
			.arg(iSubnet2)
			.arg(iSubnet3)
			.arg(iNetmask)
			.arg(iFirstIP)
			.arg(iLastIP)
			.arg(szOldName);

	query.exec(strSQL);
}

void cMainWindow::on_m_lpMenuActionVerify_triggered()
{
	verifyIPAddressList();
}

bool checkState(const Qt::CheckState& state, bool bDesiredState)
{
	if(state == Qt::PartiallyChecked)
		return(true);

	if(state == Qt::Checked && bDesiredState)
		return(true);

	if(state == Qt::Unchecked && !bDesiredState)
		return(true);

	return(false);
}

void cMainWindow::filterError()
{
	qint16		cnt		= 0;
	qint16		max		= m_lpIPAddressModel->rowCount()/200;
	QModelIndex	index	= m_lpIPAddressModel->invisibleRootItem()->index();
	QBrush		brush	= m_lpIPRangeModel->item(0, 1)->background();

	bool		bFilterError;
	bool		bFilterOldLocation;
	bool		bError	= ui->m_lpFilter->isChecked();

	m_lpProgressBar->setVisible(true);
	m_lpProgressBar->setMinimum(0);
	m_lpProgressBar->setMaximum(m_lpIPAddressModel->rowCount());

	ui->m_lpStatusBar->showMessage("filter items ...");

	for(int x = 0;x < m_ipAddressList.count();x++)
	{
		QString			szHint			= QString("");
		cIPAddress*		lpIPAddress		= m_ipAddressList.at(x);
		cIPRange*		lpIPRange		= m_ipRangeList.findRange(lpIPAddress->IPAddressBin());

		if(lpIPRange != lpIPAddress->ipRange())
		{
			lpIPAddress->setIPRange(lpIPRange);

			QStandardItem*	lpItem			= m_lpIPAddressModel->item(x, 0);
			QStandardItem*	lpItemRange		= m_lpIPAddressModel->item(x, 1);
			QStandardItem*	lpItemLocation	= m_lpIPAddressModel->item(x, 3);

			if(lpIPRange)
			{
				cLocation*	lpLocation			= m_locationList.find(lpIPRange->location());

				lpItemRange->setText(lpIPRange->name());
				if(lpLocation)
				{
					lpItemLocation->setText(lpLocation->name());

					szHint.append(QString("Name: %1").arg(lpLocation->name()));

					if(!lpLocation->location().isEmpty())
						szHint.append(QString("\nLocation: %1").arg(lpLocation->location()));

					if(!lpLocation->alternateLocation().isEmpty())
						szHint.append(QString("\nAlternate Location: %1").arg(lpLocation->alternateLocation()));

					if(!lpLocation->address().isEmpty())
						szHint.append(QString("\nAddress: %1").arg(lpLocation->address()));

					if(!lpLocation->city().isEmpty())
						szHint.append(QString("\nCity: %1").arg(lpLocation->city()));

					if(lpLocation->postalCode() > 999)
						szHint.append(QString("\nPostal Code: %1").arg(lpLocation->postalCode()));

					if(!lpLocation->federalState().isEmpty())
						szHint.append(QString("\nFederal State: %1").arg(lpLocation->federalState()));

					if(!lpLocation->country().isEmpty())
						szHint.append(QString("\nCountry: %1").arg(lpLocation->country()));
				}

				lpItem->setBackground(brush);
			}
			else
			{
				lpItemRange->setText("");
				lpItemLocation->setText("");

				lpItem->setBackground(Qt::red);
			}

			lpItemLocation->setToolTip(szHint);
		}

		if(bError)
		{
			bFilterError		= (lpIPRange == 0);
			bFilterOldLocation	= !lpIPAddress->address().isEmpty();

			bFilterError		= checkState(ui->m_lpFilterError->checkState(), bFilterError);
			bFilterOldLocation	= checkState(ui->m_lpFilterOldLocation->checkState(), bFilterOldLocation);

			if(bFilterError && bFilterOldLocation)
				ui->m_lpIPAddressList->setRowHidden(x, index, false);
			else
				ui->m_lpIPAddressList->setRowHidden(x, index, true);
		}
		else
			ui->m_lpIPAddressList->setRowHidden(x, index, false);

		cnt++;
		if(cnt > max)
		{
			cnt = 0;
			m_lpProgressBar->setValue(x);
		}
	}

	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->clearMessage();
}

void cMainWindow::on_m_lpIPRangeList_customContextMenuRequested(const QPoint &pos)
{
	QMenu*	lpMenu	= new QMenu(this);

	lpMenu->addAction("add", this, SLOT(onIPRangeAdd()));
	lpMenu->addAction("delete", this, SLOT(onIPRangeDelete()));
	lpMenu->addSeparator();
	lpMenu->addAction("add location ...", this, SLOT(onLocationAdd()));

	lpMenu->popup(ui->m_lpIPRangeList->viewport()->mapToGlobal(pos));
}

void cMainWindow::on_m_lpIPAddressList_customContextMenuRequested(const QPoint &pos)
{
	QMenu*	lpMenu	= new QMenu(this);

	lpMenu->addAction("create IP range", this, SLOT(onIPRangeCreate()));
	lpMenu->addSeparator();
	lpMenu->addAction("add location ...", this, SLOT(onLocationAdd()));

	QModelIndex			index		= ui->m_lpIPAddressList->currentIndex();
	if(index.isValid())
	{
		lpMenu->addSeparator();
		lpMenu->addAction("goto IP range", this, SLOT(onGotoRange()));
	}

	lpMenu->popup(ui->m_lpIPAddressList->viewport()->mapToGlobal(pos));
}

void cMainWindow::onIPRangeAdd()
{
	cNewIPRange*	lpNewIPRange	= new cNewIPRange(this);
	lpNewIPRange->setLocation(m_locationList);
	if(lpNewIPRange->exec() == QDialog::Rejected)
	{
		delete lpNewIPRange;
		return;
	}

	QString	szRange	= lpNewIPRange->ipRange();

	if(szRange.isEmpty())
		return;

	cIPRange*				lpRange	= m_ipRangeList.add(szRange);

	if(!lpRange)
		return;

	lpRange->setLocation(lpNewIPRange->location());

	delete lpNewIPRange;

	QList<QStandardItem*>	lpItems;

	for(int z = 0;z < m_lpIPRangeModel->columnCount();z++)
		lpItems.append(new QStandardItem);

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

	m_ipRangeList.sort();
	qint32	pos	= m_ipRangeList.position(lpRange);
	m_lpIPRangeModel->insertRow(pos, lpItems);

	ui->m_lpIPRangeList->selectionModel()->setCurrentIndex(lpItems.at(0)->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
	ui->m_lpIPRangeList->scrollTo(lpItems.at(0)->index());

	QSqlQuery	query(m_db);
	QString		szSQL	= QString("INSERT INTO ip_range_location (name, location_id, subnet, prefix, subnet1, subnet2, subnet3, mask, iplow, iphigh) VALUES ("
								  "'%1', %2, %3, %4, %5, %6, %7, %8, %9, %10);")
								.arg(lpRange->name())
								.arg(lpRange->location())
								.arg(cIPAddress::ip2bin(lpRange->range()))
								.arg(lpRange->prefix())
								.arg(lpRange->subnet1BIN())
								.arg(lpRange->subnet2BIN())
								.arg(lpRange->subnet3BIN())
								.arg(lpRange->netmaskBin())
								.arg(lpRange->firstIPAddressBin())
								.arg(lpRange->lastIPAddressBin());

	if(!query.exec(szSQL))
		qDebug() << "INSERT INTO ip_range_location" << query.lastError().text();

	filterError();
}

void cMainWindow::onIPRangeCreate()
{
	QModelIndex			index		= ui->m_lpIPAddressList->currentIndex();
	if(!index.isValid())
		return;

	QStandardItem*		lpItem		= m_lpIPAddressModel->itemFromIndex(m_lpIPAddressModel->index(index.row(), 0));
	if(!lpItem)
		return;

	cIPAddress*			lpIPAddress	= qvariant_cast<cIPAddress*>(lpItem->data(Qt::UserRole));
	if(!lpIPAddress)
		return;

	QString				szIPAddress	= lpIPAddress->IPAddress();
	QString				szRange1	= "0.0.0.0/24";

	lpItem							= m_lpIPAddressModel->itemFromIndex(m_lpIPAddressModel->index(index.row(), 2));
	QString				szLocation;

	if(lpItem)
		szLocation					= lpItem->text();

	if(szIPAddress.lastIndexOf("."))
	{
		szRange1	= szIPAddress.left(szIPAddress.lastIndexOf("."));
		szRange1.append(".0/24");
	}

	cNewIPRange*	lpNewIPRange	= new cNewIPRange(this);
	lpNewIPRange->setIPRange(szRange1);
	lpNewIPRange->setLocation(m_locationList, szLocation);
	if(lpNewIPRange->exec() == QDialog::Rejected)
	{
		delete lpNewIPRange;
		return;
	}

	QString	szRange	= lpNewIPRange->ipRange();

	if(szRange.isEmpty())
		return;

	cIPRange*				lpRange	= m_ipRangeList.add(szRange);

	if(!lpRange)
		return;

	lpRange->setLocation(lpNewIPRange->location());

	delete lpNewIPRange;

	QList<QStandardItem*>	lpItems;

	for(int z = 0;z < m_lpIPRangeModel->columnCount();z++)
		lpItems.append(new QStandardItem);

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

	m_ipRangeList.sort();
	qint32	pos	= m_ipRangeList.position(lpRange);
	m_lpIPRangeModel->insertRow(pos, lpItems);

	ui->m_lpIPRangeList->selectionModel()->setCurrentIndex(lpItems.at(0)->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
	ui->m_lpIPRangeList->scrollTo(lpItems.at(0)->index());

	QSqlQuery	query(m_db);
	QString		szSQL	= QString("INSERT INTO ip_range_location (name, location_id, subnet, prefix, subnet1, subnet2, subnet3, mask, iplow, iphigh) VALUES ("
								  "'%1', %2, %3, %4, %5, %6, %7, %8, %9, %10);")
								.arg(lpRange->name())
								.arg(lpRange->location())
								.arg(cIPAddress::ip2bin(lpRange->range()))
								.arg(lpRange->prefix())
								.arg(lpRange->subnet1BIN())
								.arg(lpRange->subnet2BIN())
								.arg(lpRange->subnet3BIN())
								.arg(lpRange->netmaskBin())
								.arg(lpRange->firstIPAddressBin())
								.arg(lpRange->lastIPAddressBin());

	if(!query.exec(szSQL))
		qDebug() << "INSERT INTO ip_range_location" << query.lastError().text();

	filterError();
}

void cMainWindow::onIPRangeDelete()
{
	if(!ui->m_lpIPRangeList->selectionModel()->selectedRows().count())
		return;

	QStandardItem*	lpItem	= m_lpIPRangeModel->itemFromIndex(ui->m_lpIPRangeList->selectionModel()->selectedIndexes().at(0));
	if(!lpItem)
		return;

	cIPRange*		lpRange	= qvariant_cast<cIPRange*>(lpItem->data(Qt::UserRole));
	if(!lpRange)
		return;

	if(QMessageBox::question(this, "delete", QString("Do you want to delete range %1?").arg(lpRange->name())) == QMessageBox::No)
		return;

	QSqlQuery	query(m_db);
	QString		szQuery	= QString("DELETE FROM ip_range_location WHERE name='%1';").arg(lpRange->name());
	if(!query.exec(szQuery))
	{
		qDebug() << "DELETE FROM ip_range_location" << query.lastError().text();
		return;
	}

	m_ipRangeList.removeOne(lpRange);
	m_lpIPRangeModel->removeRow(lpItem->index().row(), m_lpIPRangeModel->invisibleRootItem()->index());

	filterError();
}

void cMainWindow::onLocationAdd()
{
	QString	szLocation;

	if(ui->m_lpTab->currentIndex() == 1)
	{
		if(ui->m_lpIPAddressList->selectionModel()->selectedRows().count())
		{
			QStandardItem*	lpItem		= m_lpIPAddressModel->itemFromIndex(ui->m_lpIPAddressList->selectionModel()->selectedIndexes().at(0));
			if(lpItem)
			{
				cIPAddress*	lpIPAddress	= qvariant_cast<cIPAddress*>(lpItem->data(Qt::UserRole));
				if(lpIPAddress)
					szLocation	= lpIPAddress->address();
			}
		}
	}

	cAddLocationDialog*	lpAddLocationDialog	= new cAddLocationDialog(this);

	if(!szLocation.isEmpty())
		lpAddLocationDialog->setLocation(szLocation);

	if(lpAddLocationDialog->exec() == QDialog::Rejected)
	{
		delete lpAddLocationDialog;
		return;
	}

	delete lpAddLocationDialog;

	on_m_lpMenuFileReloadLocationList_triggered();
}

void cMainWindow::on_m_lpMenuFileLoadClientsFromDB_triggered()
{
	m_ipAddressList.clear();

	QSqlDatabase	dbMySQL	= QSqlDatabase::addDatabase("QMYSQL", "mysql");
	dbMySQL.setHostName(m_szHostName);
	dbMySQL.setDatabaseName(m_szDatabaseName);
	dbMySQL.setUserName(m_szUserName);
	dbMySQL.setPassword(m_szPassword);
	if(!dbMySQL.open())
		qDebug() << dbMySQL.lastError().text();
	else
	{
		QSqlQuery	query(dbMySQL);
		QString		szSQL	= QString("SELECT DISTINCT clientip, location FROM clientip_list ORDER BY INET_ATON(clientip);");
		if(!query.exec(szSQL))
			qDebug() << query.lastError().text();

		m_lpProgressBar->setVisible(true);
		m_lpProgressBar->setMinimum(0);
		m_lpProgressBar->setMaximum(query.size());

		qint16		cnt			= 0;
		qint16		max			= query.size()/200;
		qint32		index		= 0;

		ui->m_lpStatusBar->showMessage("loading IP address list ...");

		while(query.next())
		{
			m_ipAddressList.add(query.value("clientip").toString(), query.value("location").toString());

			index++;
			cnt++;
			if(cnt > max)
			{
				cnt = 0;
				m_lpProgressBar->setValue(index);
			}
		}

		m_lpProgressBar->setVisible(false);

		ui->m_lpStatusBar->clearMessage();

		dbMySQL.close();
	}
	m_ipAddressList.sort();
	displayIPAddressList();
}


void cMainWindow::on_m_lpMenuImportIPRangesFromExcel_triggered()
{
	cImportExcelDialog*	lpImportExcelDialog	= new cImportExcelDialog(this);
	lpImportExcelDialog->exec();
	delete lpImportExcelDialog;
}

void cMainWindow::on_m_lpMenuFileReloadLocationList_triggered()
{
	loadLocationList();
	loadIPRangeList();
	displayIPRangeList();
	filterError();
}

void cMainWindow::on_m_lpFilter_clicked()
{
	ui->m_lpFilterError->setEnabled(ui->m_lpFilter->isChecked());
	ui->m_lpFilterOldLocation->setEnabled(ui->m_lpFilter->isChecked());

	filterError();
}

void cMainWindow::on_m_lpFilterError_clicked()
{
	filterError();
}

void cMainWindow::on_m_lpFilterOldLocation_clicked()
{
	filterError();
}

void cMainWindow::on_m_lpMenuFileExportToSQL_triggered()
{
	QDir	dir;
	QString	strHome		= dir.homePath() + QDir::separator();
	QString	strFileName = QFileDialog::getSaveFileName(this, tr("Save to SQL file"), strHome, tr("SQL Files (*.sql)"));

	if(strFileName.isEmpty())
		return;

	QFile	file(strFileName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	qint16		cnt			= 0;
	qint16		max			= m_ipRangeList.count()/200;

	m_lpProgressBar->setVisible(true);
	m_lpProgressBar->setMinimum(0);
	m_lpProgressBar->setMaximum(m_ipRangeList.count());

	ui->m_lpStatusBar->showMessage("exporting IP address list ...");

	QTextStream out(&file);

	out << "DROP TABLE IF EXISTS ip_range_location_backup;\n\n";
	out << "CREATE TABLE ip_range_location_backup AS SELECT * FROM ip_range_location;\n\n";
	out << "TRUNCATE TABLE ip_range_location;\n\n";

	for(int x = 0;x < m_ipRangeList.count();x++)
	{
		cIPRange*	lpRange	= m_ipRangeList.at(x);

		if(lpRange->location() == -1)
			continue;

		out << "INSERT INTO ip_range_location (`name`, `location_id`, `subnet`, `prefix`, `subnet1`, `subnet2`, `subnet3`, `mask`, `iplow`, `iphigh`) VALUES (";
		out << "'" << lpRange->name() << "', ";
		out << lpRange->location() << ", ";
		out << lpRange->subnetBIN() << ", ";
		out << lpRange->prefix() << ", ";
		out << lpRange->subnet1BIN() <<", ";
		out << lpRange->subnet2BIN() <<", ";
		out << lpRange->subnet3BIN() <<", ";
		out << lpRange->netmaskBin() << ", ";
		out << lpRange->firstIPAddressBin() << ", ";
		out << lpRange->lastIPAddressBin() << ");\n";

		cnt++;
		if(cnt > max)
		{
			cnt = 0;
			m_lpProgressBar->setValue(x);
		}
	}

	file.close();

	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->clearMessage();
}

void cMainWindow::on_m_lpMenuFileExportToDatabase_triggered()
{
	QSqlDatabase	dbMySQL	= QSqlDatabase::addDatabase("QMYSQL", "mysql");
	dbMySQL.setHostName(m_szHostName);
	dbMySQL.setDatabaseName(m_szDatabaseName);
	dbMySQL.setUserName(m_szUserName);
	dbMySQL.setPassword(m_szPassword);
	if(!dbMySQL.open())
	{
		qDebug() << dbMySQL.lastError().text();
		return;
	}

	QSqlQuery		query(dbMySQL);

	qint16		cnt			= 0;
	qint16		max			= m_ipRangeList.count()/200;

	m_lpProgressBar->setVisible(true);
	m_lpProgressBar->setMinimum(0);
	m_lpProgressBar->setMaximum(m_ipRangeList.count());

	ui->m_lpStatusBar->showMessage("exporting IP address list ...");

	if(!query.exec("DROP TABLE IF EXISTS ip_range_location_backup;"))
	{
		qDebug() << dbMySQL.lastError().text();
		dbMySQL.close();
		return;
	}

	if(!query.exec("CREATE TABLE ip_range_location_backup AS SELECT * FROM ip_range_location;"))
	{
		qDebug() << dbMySQL.lastError().text();
		dbMySQL.close();
		return;
	}

	if(!query.exec("TRUNCATE TABLE ip_range_location;"))
	{
		qDebug() << dbMySQL.lastError().text();
		dbMySQL.close();
		return;
	}

	query.prepare("INSERT INTO ip_range_location (`name`, `location_id`, `subnet`, `prefix`, `subnet1`, `subnet2`, `subnet3`, `mask`, `iplow`, `iphigh`)"
						  " VALUES (:name,:location_id,:subnet,:prefix,:subnet1,:subnet2,:subnet3,:mask,:iplow,:iphigh);");

	dbMySQL.transaction();

	for(int x = 0;x < m_ipRangeList.count();x++)
	{
		cIPRange*	lpRange	= m_ipRangeList.at(x);

		if(lpRange->location() == -1)
			continue;

		query.bindValue(":name", lpRange->name());
		query.bindValue(":location_id", lpRange->location());
		query.bindValue(":subnet", lpRange->subnetBIN());
		query.bindValue(":prefix", lpRange->prefix());
		query.bindValue(":subnet1", lpRange->subnet1BIN());
		query.bindValue(":subnet2", lpRange->subnet2BIN());
		query.bindValue(":subnet3", lpRange->subnet3BIN());
		query.bindValue(":mask", lpRange->netmaskBin());
		query.bindValue(":iplow", lpRange->firstIPAddressBin());
		query.bindValue(":iphigh", lpRange->lastIPAddressBin());

		if(!query.exec())
			qDebug() << query.lastError().text();

		cnt++;
		if(cnt > max)
		{
			cnt = 0;
			m_lpProgressBar->setValue(x);
		}
	}

	dbMySQL.commit();
	dbMySQL.close();

	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->clearMessage();
}

void cMainWindow::on_m_lpMenuFileExportToCSV_triggered()
{
	QDir	dir;
	QString	strHome		= dir.homePath() + QDir::separator();
	QString	strFileName = QFileDialog::getSaveFileName(this, tr("Save to CSV file"), strHome, tr("CSV Files (*.csv)"));

	if(strFileName.isEmpty())
		return;

	QFile	file(strFileName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	qint16		cnt			= 0;
	qint16		max			=m_lpIPRangeModel->rowCount()/200;

	m_lpProgressBar->setVisible(true);
	m_lpProgressBar->setMinimum(0);
	m_lpProgressBar->setMaximum(m_ipRangeList.count());

	ui->m_lpStatusBar->showMessage("exporting IP address list ...");

	QTextStream out(&file);

	out << "range;base ip;prefix;subnet mask;broadcast ip;first ip;last ip;location\n";

	for(int x = 0;x < m_lpIPRangeModel->rowCount();x++)
	{
		QStandardItem*	lpRange			= m_lpIPRangeModel->item(x, 1);
		QStandardItem*	lpBaseIP		= m_lpIPRangeModel->item(x, 2);
		QStandardItem*	lpPrefix		= m_lpIPRangeModel->item(x, 3);
		QStandardItem*	lpSubnetMask	= m_lpIPRangeModel->item(x, 4);
		QStandardItem*	lpBroadcastIP	= m_lpIPRangeModel->item(x, 5);
		QStandardItem*	lpFirstIP		= m_lpIPRangeModel->item(x, 6);
		QStandardItem*	lpLastIP		= m_lpIPRangeModel->item(x, 7);
		QStandardItem*	lpLocation		= m_lpIPRangeModel->item(x, 8);

		out << "\"" << lpRange->text() << "\";";
		out << "\"" << lpBaseIP->text() << "\";";
		out << "\"" << lpPrefix->text() << "\";";
		out << "\"" << lpSubnetMask->text() << "\";";
		out << "\"" << lpBroadcastIP->text() << "\";";
		out << "\"" << lpFirstIP->text() << "\";";
		out << "\"" << lpLastIP->text() << "\";";
		out << "\"" << lpLocation->text() << "\"\n";

		cnt++;
		if(cnt > max)
		{
			cnt = 0;
			m_lpProgressBar->setValue(x);
		}
	}

	file.close();

	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->clearMessage();
}

void cMainWindow::on_m_lpMenuFileExportToExcel_triggered()
{
	QDir	dir;
	QString	strHome		= dir.homePath() + QDir::separator();
	QString	strFileName = QFileDialog::getSaveFileName(this, tr("Save to Excel file"), strHome, tr("Excel Files (*.xlsx)"));

	if(strFileName.isEmpty())
		return;

	qint16		cnt			= 0;
	qint16		max			=m_lpIPRangeModel->rowCount()/200;

	m_lpProgressBar->setVisible(true);
	m_lpProgressBar->setMinimum(0);
	m_lpProgressBar->setMaximum(m_ipRangeList.count());

	ui->m_lpStatusBar->showMessage("exporting IP address list ...");

	QXlsx::Format		format;
	QXlsx::Format		formatBig;
	QXlsx::Format		formatMerged;
	QXlsx::Format		formatCurrency;
	QXlsx::Format		formatNumber;
	QXlsx::Format		formatDate;
	QXlsx::Format		formatWrap;

	QString				szNumberFormatCurrency("_-\"€\"\\ * #,##0.00_-;\\-\"€\"\\ * #,##0.00_-;_-\"€\"\\ * \"-\"??_-;_-@_-");
	QString				szNumberFormatNumber("#,##0.00");
	QString				szNumberFormatDate("dd.mm.yyyy");

	format.setFontBold(true);

	formatBig.setFontBold(true);
	formatBig.setFontSize(16);

	formatMerged.setHorizontalAlignment(QXlsx::Format::AlignLeft);
	formatMerged.setVerticalAlignment(QXlsx::Format::AlignTop);

	formatCurrency.setNumberFormat(szNumberFormatCurrency);
	formatNumber.setNumberFormat(szNumberFormatNumber);
	formatDate.setNumberFormat(szNumberFormatDate);

	formatWrap.setTextWarp(true);

	QXlsx::Document			xlsx;
	QXlsx::Format			fmt;
	fmt.setFontBold(true);
	xlsx.write(1, 1, "empty");
	xlsx.write(2, 1, "empty", fmt);
	QFont					font		= xlsx.cellAt(1, 1)->format().font();
	QFont					fontBold	= xlsx.cellAt(2, 1)->format().font();
	fmt.setFontBold(false);
	xlsx.write(1, 1, "");
	xlsx.write(2, 1, "", fmt);

	xlsx.write(1, 1, "range");
	xlsx.write(1, 2, "base ip");
	xlsx.write(1, 3, "prefix");
	xlsx.write(1, 4, "subnet mask");
	xlsx.write(1, 5, "broadcast ip");
	xlsx.write(1, 6, "first ip");
	xlsx.write(1, 7, "last ip");
	xlsx.write(1, 8, "location");

	for(int x = 0;x < m_lpIPRangeModel->rowCount();x++)
	{
		QStandardItem*	lpRange			= m_lpIPRangeModel->item(x, 1);
		QStandardItem*	lpBaseIP		= m_lpIPRangeModel->item(x, 2);
		QStandardItem*	lpPrefix		= m_lpIPRangeModel->item(x, 3);
		QStandardItem*	lpSubnetMask	= m_lpIPRangeModel->item(x, 4);
		QStandardItem*	lpBroadcastIP	= m_lpIPRangeModel->item(x, 5);
		QStandardItem*	lpFirstIP		= m_lpIPRangeModel->item(x, 6);
		QStandardItem*	lpLastIP		= m_lpIPRangeModel->item(x, 7);
		QStandardItem*	lpLocation		= m_lpIPRangeModel->item(x, 8);

		xlsx.write(x+2,  1, lpRange->text());
		xlsx.write(x+2,  2, lpBaseIP->text());
		xlsx.write(x+2,  3, lpPrefix->text());
		xlsx.write(x+2,  4, lpSubnetMask->text());
		xlsx.write(x+2,  5, lpBroadcastIP->text());
		xlsx.write(x+2,  6, lpFirstIP->text());
		xlsx.write(x+2,  7, lpLastIP->text());
		xlsx.write(x+2,  8, lpLocation->text());

		cnt++;
		if(cnt > max)
		{
			cnt = 0;
			m_lpProgressBar->setValue(x);
		}
	}

	xlsx.saveAs(strFileName);

	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->clearMessage();
}

void cMainWindow::on_m_lpMenuImportIPRangesFromDB_triggered()
{
	QSqlDatabase	dbMySQL	= QSqlDatabase::addDatabase("QMYSQL", "mysql");
	dbMySQL.setHostName(m_szHostName);
	dbMySQL.setDatabaseName(m_szDatabaseName);
	dbMySQL.setUserName(m_szUserName);
	dbMySQL.setPassword(m_szPassword);
	if(!dbMySQL.open())
	{
		qDebug() << dbMySQL.lastError().text();
		return;
	}

	QSqlQuery		queryMySQL(dbMySQL);
	QSqlQuery		query(m_db);

	qint16		cnt			= 0;
	qint16		max			= m_ipRangeList.count()/200;
	qint16		x			= 0;

	m_lpProgressBar->setVisible(true);
	m_lpProgressBar->setMinimum(0);

	ui->m_lpStatusBar->showMessage("importing IP address list ...");

	if(!query.exec("DROP TABLE IF EXISTS ip_range_location_backup;"))
	{
		qDebug() << dbMySQL.lastError().text();
		return;
	}

	if(!query.exec("CREATE TABLE ip_range_location_backup AS SELECT * FROM ip_range_location;"))
	{
		qDebug() << dbMySQL.lastError().text();
		return;
	}

	if(!query.exec("DELETE FROM ip_range_location;"))
	{
		qDebug() << dbMySQL.lastError().text();
		return;
	}

	query.prepare("INSERT INTO ip_range_location (`name`, `location_id`, `subnet`, `prefix`, `subnet1`, `subnet2`, `subnet3`, `mask`, `iplow`, `iphigh`)"
						  " VALUES (:name,:location_id,:subnet,:prefix,:subnet1,:subnet2,:subnet3,:mask,:iplow,:iphigh);");


	QString	szSQL	= QString("SELECT name, location_id, subnet, prefix, subnet1, subnet2, subnet3, mask, iplow, iphigh FROM ip_range_location ORDER BY INET_ATON(subnet);");
	if(!queryMySQL.exec(szSQL))
	{
		qDebug() << dbMySQL.lastError().text();
		return;
	}
	m_lpProgressBar->setMaximum(queryMySQL.size());

	m_db.transaction();

	while(queryMySQL.next())
	{
		query.bindValue(":name", queryMySQL.value("name").toString());
		query.bindValue(":location_id", queryMySQL.value("location_id").toInt());
		query.bindValue(":subnet", queryMySQL.value("subnet").toInt());
		query.bindValue(":prefix", queryMySQL.value("prefix").toInt());
		query.bindValue(":subnet1", queryMySQL.value("subnet1").toInt());
		query.bindValue(":subnet2", queryMySQL.value("subnet2").toInt());
		query.bindValue(":subnet3", queryMySQL.value("subnet3").toInt());
		query.bindValue(":mask", queryMySQL.value("mask").toInt());
		query.bindValue(":iplow", queryMySQL.value("iplow").toInt());
		query.bindValue(":iphigh", queryMySQL.value("iphigh").toInt());

		if(!query.exec())
			qDebug() << query.lastError().text();

		cnt++;
		if(cnt > max)
		{
			cnt = 0;
			m_lpProgressBar->setValue(x);
		}
		x++;
	}

	m_db.commit();
	dbMySQL.close();

	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->clearMessage();

	loadIPRangeList();
	displayIPRangeList();
	filterError();
}

void cMainWindow::on_m_lpTab_currentChanged(int index)
{
	if(index)
	{
		ui->m_lpMenuActionFindPreviousError->setEnabled(true);
		ui->m_lpMenuActionFindNextError->setEnabled(true);
	}
	else
	{
		ui->m_lpMenuActionFindPreviousError->setEnabled(false);
		ui->m_lpMenuActionFindNextError->setEnabled(false);
	}
}

void cMainWindow::on_m_lpMenuActionFind_triggered()
{
	if(ui->m_lpTab->currentIndex())
		findIPAddress();
	else
		findIPRange();
}

void cMainWindow::on_m_lpMenuActionFindNextError_triggered()
{
	if(!m_lpIPAddressModel->rowCount())
		return;

	QModelIndex	index		= ui->m_lpIPAddressList->currentIndex();
	qint32		iSelected	= -1;
	QModelIndex	parent		= m_lpIPAddressModel->invisibleRootItem()->index();

	if(index.isValid())
		iSelected	= index.row();

	iSelected++;
	if(iSelected == m_lpIPAddressModel->rowCount())
		iSelected = 0;

	qint32		iFirst		= iSelected;

	for(int x = iSelected;x < m_lpIPAddressModel->rowCount();x++)
	{
		QStandardItem*	lpItem		= m_lpIPAddressModel->item(x, 0);

		if(lpItem->background() == Qt::red && !ui->m_lpIPAddressList->isRowHidden(x, parent))
		{
			ui->m_lpIPAddressList->selectionModel()->setCurrentIndex(lpItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
			ui->m_lpIPAddressList->scrollTo(lpItem->index());
			return;
		}
	}

	if(iFirst)
	{
		for(int x = 0;x < iFirst;x++)
		{
			QStandardItem*	lpItem		= m_lpIPAddressModel->item(x, 0);

			if(lpItem->background() == Qt::red && !ui->m_lpIPAddressList->isRowHidden(x, parent))
			{
				ui->m_lpIPAddressList->selectionModel()->setCurrentIndex(lpItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
				ui->m_lpIPAddressList->scrollTo(lpItem->index());
				return;
			}
		}
	}

	QMessageBox::information(this, "Find", "Nothing found.");
}

void cMainWindow::on_m_lpMenuActionFindPreviousError_triggered()
{
	if(!m_lpIPAddressModel->rowCount())
		return;

	QModelIndex	index		= ui->m_lpIPAddressList->currentIndex();
	qint32		iSelected	= m_lpIPAddressModel->rowCount();
	QModelIndex	parent		= m_lpIPAddressModel->invisibleRootItem()->index();

	if(index.isValid())
		iSelected	= index.row();

	iSelected--;
	if(iSelected < 0)
		iSelected = m_lpIPAddressModel->rowCount()-1;

	qint32		iFirst		= iSelected;

	for(int x = iSelected;x >= 0;x--)
	{
		QStandardItem*	lpItem		= m_lpIPAddressModel->item(x, 0);

		if(lpItem->background() == Qt::red && !ui->m_lpIPAddressList->isRowHidden(x, parent))
		{
			ui->m_lpIPAddressList->selectionModel()->setCurrentIndex(lpItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
			ui->m_lpIPAddressList->scrollTo(lpItem->index());
			return;
		}
	}

	if(iFirst)
	{
		for(int x = m_lpIPAddressModel->rowCount()-1;x > iFirst;x--)
		{
			QStandardItem*	lpItem		= m_lpIPAddressModel->item(x, 0);

			if(lpItem->background() == Qt::red && !ui->m_lpIPAddressList->isRowHidden(x, parent))
			{
				ui->m_lpIPAddressList->selectionModel()->setCurrentIndex(lpItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
				ui->m_lpIPAddressList->scrollTo(lpItem->index());
				return;
			}
		}
	}

	QMessageBox::information(this, "Find", "Nothing found.");
}

void cMainWindow::on_m_lpMenuActionFindNext_triggered()
{
	if(ui->m_lpTab->currentIndex())
	{
		if(m_szIPAddressFindText.isEmpty())
			findIPAddress();
		else
			findNextIPAddress();
	}
	else
	{
		if(m_szIPRangeFindText.isEmpty())
			findIPRange();
		else
			findNextIPRange();
	}
}

void cMainWindow::findIPRange()
{
	cFindDialogIPRange*	lpFind	= new cFindDialogIPRange(this);
	lpFind->setFindText(m_szIPRangeFindText);
	lpFind->setFindFlags(m_IPRangeFindFlags);

	if(lpFind->exec() == QDialog::Rejected)
	{
		delete lpFind;
		return;
	}

	m_szIPRangeFindText	= lpFind->findText();
	m_IPRangeFindFlags	= lpFind->findFlags();

	delete lpFind;

	findNextIPRange();
}

void cMainWindow::findNextIPRange()
{
	if(!m_lpIPRangeModel->rowCount())
		return;

	QModelIndex	index		= ui->m_lpIPRangeList->currentIndex();
	qint32		iSelected	= -1;

	if(index.isValid())
		iSelected	= index.row();

	iSelected++;
	if(iSelected == m_lpIPRangeModel->rowCount())
		iSelected = 0;

	qint32		iFirst		= iSelected;

	for(int x = iSelected;x < m_lpIPRangeModel->rowCount();x++)
	{
		QStandardItem*	lpItem		= m_lpIPRangeModel->item(x, 0);
		cIPRange*		lpRange		= qvariant_cast<cIPRange*>(lpItem->data(Qt::UserRole));

		if(!lpRange)
			continue;

		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInName))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInRange))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInBaseIP))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInPrefix))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInSubnetMask))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInBroadcastIP))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInFirstIP))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInLastIP))
			return;
		if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInLocation))
			return;
	}

	if(iFirst)
	{
		for(int x = 0;x < iFirst;x++)
		{
			QStandardItem*	lpItem		= m_lpIPRangeModel->item(x, 0);
			cIPRange*		lpRange		= qvariant_cast<cIPRange*>(lpItem->data(Qt::UserRole));

			if(!lpRange)
				continue;

			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInName))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInRange))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInBaseIP))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInPrefix))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInSubnetMask))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInBroadcastIP))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInFirstIP))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInLastIP))
				return;
			if(matchIPRange(lpItem, lpRange, cFindDialogIPRange::FindInLocation))
				return;
		}
	}

	QMessageBox::information(this, "Find", "Nothing found.");
}

void cMainWindow::findIPAddress()
{
	cFindDialogIPAddress*	lpFind	= new cFindDialogIPAddress(this);
	lpFind->setFindText(m_szIPAddressFindText);
	lpFind->setFindFlags(m_IPAddressFindFlags);

	if(lpFind->exec() == QDialog::Rejected)
	{
		delete lpFind;
		return;
	}

	m_szIPAddressFindText	= lpFind->findText();
	m_IPAddressFindFlags	= lpFind->findFlags();

	delete lpFind;

	findNextIPAddress();
}

void cMainWindow::findNextIPAddress()
{
	if(!m_lpIPAddressModel->rowCount())
		return;

	QModelIndex	index		= ui->m_lpIPAddressList->currentIndex();
	qint32		iSelected	= -1;

	if(index.isValid())
		iSelected	= index.row();

	iSelected++;
	if(iSelected == m_lpIPAddressModel->rowCount())
		iSelected = 0;

	qint32		iFirst		= iSelected;

	for(int x = iSelected;x < m_lpIPAddressModel->rowCount();x++)
	{
		QStandardItem*	lpItem		= m_lpIPAddressModel->item(x, 0);
		cIPAddress*		lpIPAddress	= qvariant_cast<cIPAddress*>(lpItem->data(Qt::UserRole));

		if(!lpIPAddress)
			continue;

		if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInIPAddress))
			return;
		if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInRange))
			return;
		if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInOriLocation))
			return;
		if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInLocation))
			return;
	}

	if(iFirst)
	{
		for(int x = 0;x < iFirst;x++)
		{
			QStandardItem*	lpItem		= m_lpIPAddressModel->item(x, 0);
			cIPAddress*		lpIPAddress	= qvariant_cast<cIPAddress*>(lpItem->data(Qt::UserRole));

			if(!lpIPAddress)
				continue;

			if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInIPAddress))
				return;
			if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInRange))
				return;
			if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInOriLocation))
				return;
			if(matchIPAddress(lpItem, lpIPAddress, cFindDialogIPAddress::FindInLocation))
				return;
		}
	}

	QMessageBox::information(this, "Find", "Nothing found.");
}

bool cMainWindow::matchIPRange(QStandardItem* lpItem, cIPRange* lpRange, cFindDialogIPRange::FindIn flag)
{
	bool	bFound	= false;

	if(m_IPRangeFindFlags & flag)
	{
		switch(flag)
		{
		case cFindDialogIPRange::FindInName:
			if(lpRange->name().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInRange:
			if(lpRange->range().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInBaseIP:
			if(lpRange->IPAddress().IPAddress().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInPrefix:
			if(QString("%1").arg(lpRange->prefix()).contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInSubnetMask:
			if(lpRange->netmask().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInBroadcastIP:
			if(lpRange->broadcastIPAddress().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInFirstIP:
			if(lpRange->firstIPAddress().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInLastIP:
			if(lpRange->lastIPAddress().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPRange::FindInLocation:
		{
			cLocation*	lpLocation	= m_locationList.find(lpRange->location());
			if(lpLocation)
			{
				if(lpLocation->name().contains(m_szIPRangeFindText, Qt::CaseInsensitive))
					bFound	= true;
			}
			break;
		}
		case cFindDialogIPRange::FindInAll:
			break;
		}
	}

	if(bFound)
	{
		ui->m_lpIPRangeList->selectionModel()->setCurrentIndex(lpItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
		ui->m_lpIPRangeList->scrollTo(lpItem->index());
	}
	return(bFound);
}

bool cMainWindow::matchIPAddress(QStandardItem* lpItem, cIPAddress *lpIPAddress, cFindDialogIPAddress::FindIn flag)
{
	bool		bFound	= false;
	cIPRange*	lpIPRange	= m_ipRangeList.findRange(lpIPAddress->IPAddressBin());

	if(m_IPAddressFindFlags & flag)
	{
		switch(flag)
		{
		case cFindDialogIPAddress::FindInIPAddress:
			if(lpIPAddress->IPAddress().contains(m_szIPAddressFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPAddress::FindInRange:
			if(lpIPRange)
			{
				if(lpIPRange->name().contains(m_szIPAddressFindText, Qt::CaseInsensitive))
					bFound	= true;
			}
			break;
		case cFindDialogIPAddress::FindInOriLocation:
			if(lpIPAddress->address().contains(m_szIPAddressFindText, Qt::CaseInsensitive))
				bFound	= true;
			break;
		case cFindDialogIPAddress::FindInLocation:
		{
			if(!lpIPRange)
				break;
			cLocation*	lpLocation	= m_locationList.find(lpIPRange->location());
			if(lpLocation)
			{
				if(lpLocation->name().contains(m_szIPAddressFindText, Qt::CaseInsensitive))
					bFound	= true;
			}
			break;
		}
		case cFindDialogIPAddress::FindInAll:
			break;
		}
	}

	if(bFound)
	{
		ui->m_lpIPAddressList->selectionModel()->setCurrentIndex(lpItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
		ui->m_lpIPAddressList->scrollTo(lpItem->index());
	}
	return(bFound);
}

void cMainWindow::onGotoRange()
{
	QModelIndex			index1		= ui->m_lpIPAddressList->currentIndex();
	if(!index1.isValid())
		return;

	QStandardItem*		lpItem1		= m_lpIPAddressModel->itemFromIndex(m_lpIPAddressModel->index(index1.row(),1));
	if(!lpItem1)
		return;

	QString				szIPRange	= lpItem1->text();

	if(szIPRange.isEmpty())
		return;

	if(!m_lpIPRangeModel->rowCount())
		return;

	for(int x = 0;x < m_lpIPRangeModel->rowCount();x++)
	{
		QStandardItem*	lpItem		= m_lpIPRangeModel->item(x, 0);
		cIPRange*		lpRange		= qvariant_cast<cIPRange*>(lpItem->data(Qt::UserRole));

		if(!lpRange)
			continue;

		if(lpRange->name() == szIPRange)
		{
			ui->m_lpIPRangeList->selectionModel()->setCurrentIndex(lpItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
			ui->m_lpIPRangeList->scrollTo(lpItem->index());
			ui->m_lpTab->setCurrentIndex(0);
			return;
		}
	}

	QMessageBox::information(this, "Find", "Nothing found.");
}
