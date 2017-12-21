#include "caddlocationdialog.h"
#include "ui_caddlocationdialog.h"

#include "caddcitydialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>


cAddLocationDialog::cAddLocationDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cAddLocationDialog)
{
	ui->setupUi(this);

	m_dbMySQL	= QSqlDatabase::addDatabase("QMYSQL", "mysql");
	m_dbMySQL.setHostName("10.69.208.60");
	m_dbMySQL.setDatabaseName("reporting");
	m_dbMySQL.setUserName("reporting");
	m_dbMySQL.setPassword("reporting");
	if(!m_dbMySQL.open())
	{
		qDebug() << m_dbMySQL.lastError().text();
		return;
	}

	loadCountryList();
	loadFederalStateList();
	loadCityList();
}

void cAddLocationDialog::loadCountryList()
{
	ui->m_lpCountry->clear();

	QSqlQuery	query(m_dbMySQL);

	if(!query.exec("SELECT id, name FROM country ORDER BY name"))
		qDebug() << query.lastError().text();

	ui->m_lpCountry->addItem("-", QVariant::fromValue(-1));
	while(query.next())
		ui->m_lpCountry->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
}

void cAddLocationDialog::loadFederalStateList()
{
	ui->m_lpFederalState->clear();

	QSqlQuery	query(m_dbMySQL);

	if(!query.exec("SELECT id, name FROM federal_state ORDER BY name"))
		qDebug() << query.lastError().text();

	ui->m_lpFederalState->addItem("-", QVariant::fromValue(-1));
	while(query.next())
		ui->m_lpFederalState->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
}

void cAddLocationDialog::loadCityList()
{
	ui->m_lpCity->clear();

	QSqlQuery	query(m_dbMySQL);

	if(!query.exec("SELECT id, name, postal_code FROM city ORDER BY name"))
		qDebug() << query.lastError().text();

	ui->m_lpCity->addItem("-", QVariant::fromValue(-1));
	while(query.next())
	{
		QString	str;

		if(query.value("postal_code").toInt() > 999)
			str	= QString("%1 (%2)").arg(query.value("name").toString()).arg(query.value("postal_code").toInt());
		else
			str	= query.value("name").toString();

		ui->m_lpCity->addItem(str, QVariant::fromValue(query.value("id").toInt()));
	}
}

cAddLocationDialog::~cAddLocationDialog()
{
	if(m_dbMySQL.isOpen())
		m_dbMySQL.close();
	delete ui;
}

void cAddLocationDialog::on_m_lpAddCountry_clicked()
{
	bool	bOK;
	QString	szCountry	= QInputDialog::getText(this, "New Country", "Country:", QLineEdit::Normal, "", &bOK);

	if(!bOK || szCountry.isEmpty())
		return;

	QSqlQuery	query(m_dbMySQL);

	if(!query.exec(QString("SELECT id FROM country WHERE name='%1';").arg(szCountry)))
	{
		qDebug() << query.lastError().text();
		return;
	}

	if(query.size())
	{
		QMessageBox::critical(this, "Error", "Country already exists.");
		return;
	}

	if(!query.exec(QString("INSERT INTO country (name) VALUES ('%1');").arg(szCountry)))
	{
		qDebug() << query.lastError().text();
		return;
	}

	if(!query.exec(QString("SELECT id FROM country WHERE name='%1'").arg(szCountry)))
	{
		qDebug() << query.lastError().text();
		return;
	}

	loadCountryList();
	ui->m_lpCountry->setCurrentText(szCountry);
}

void cAddLocationDialog::on_m_lpAddCity_clicked()
{
	cAddCityDialog*	lpAddCityDialog	= new cAddCityDialog(this);

	if(lpAddCityDialog->exec() == QDialog::Rejected)
	{
		delete lpAddCityDialog;
		return;
	}

	QString	szCity		= lpAddCityDialog->city();
	qint32	iPostalCode	= lpAddCityDialog->postalCode();

	QSqlQuery	query(m_dbMySQL);

	if(!query.exec(QString("SELECT id FROM city WHERE name='%1';").arg(szCity)))
	{
		qDebug() << query.lastError().text();
		return;
	}

	if(query.size())
	{
		QMessageBox::critical(this, "Error", "City already exists.");
		return;
	}

	QString	szSQL	= "INSERT INTO city (name";
	if(iPostalCode > 999)
		szSQL.append(QString(", postal_code) VALUES ('%2', %3);").arg(szCity).arg(iPostalCode));
	else
		szSQL.append(QString(") VALUES ('%2');").arg(szCity));

	if(!query.exec(szSQL))
	{
		qDebug() << query.lastError().text();
		return;
	}

	if(!query.exec(QString("SELECT id FROM city WHERE name='%1';").arg(szCity)))
	{
		qDebug() << query.lastError().text();
		return;
	}

	loadCityList();

	QString	str;

	if(iPostalCode > 999)
		str	= QString("%1 (%2)").arg(szCity).arg(iPostalCode);
	else
		str	= szCity;

	ui->m_lpCity->setCurrentText(str);
}

void cAddLocationDialog::accept()
{
	QString		szName			= ui->m_lpName->text();
	QString		szLocation		= ui->m_lpLocation->text();
	QString		szAddress		= ui->m_lpAddress->text();
	qint32		iCountry		= ui->m_lpCountry->currentData().toInt();

	qint32		iFederalState	= ui->m_lpFederalState->currentData().toInt();
	qint32		iCity			= ui->m_lpCity->currentData().toInt();

	if(id() != -1)
	{
		QMessageBox::critical(this, "Error", QString("The location '%1' already exists."));
		return;
	}

	QSqlQuery	query(m_dbMySQL);

	query.prepare("INSERT INTO location (name, location, address, country_id, federal_state_id, city_id) VALUES (:name, :location, :address, :country_id, :federal_state_id, :city_id);");
	query.bindValue(":name", szName);
	query.bindValue(":location", szLocation);
	if(szAddress.isEmpty())
		query.bindValue(":address", QVariant::String);
	else
		query.bindValue(":address", szAddress);
	if(iCountry == -1)
		query.bindValue(":country_id", QVariant::Int);
	else
		query.bindValue(":country_id", iCountry);
	if(iFederalState == -1)
		query.bindValue(":federal_state_id", QVariant::Int);
	else
		query.bindValue(":federal_state_id", iFederalState);
	if(iCity == -1)
		query.bindValue(":city_id", QVariant::Int);
	else
		query.bindValue(":city_id", iCity);

	if(!query.exec())
		qDebug() << query.lastError().text();

	QDialog::accept();
}

void cAddLocationDialog::on_m_lpName_textChanged(const QString &arg1)
{
	if(arg1.isEmpty())
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void cAddLocationDialog::on_m_lpLocation_textChanged(const QString &arg1)
{
	if(arg1.isEmpty())
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else
		ui->m_lpButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void cAddLocationDialog::setLocation(const QString& szLocation)
{
	ui->m_lpName->setText(szLocation);

	if(!szLocation.contains(", "))
	{
		if(ui->m_lpCountry->findText(szLocation, Qt::MatchExactly))
			ui->m_lpCountry->setCurrentText(szLocation);

		return;
	}

	QStringList	lst	= szLocation.split(", ");
	ui->m_lpLocation->setText(lst.at(0));

	if(ui->m_lpCountry->findText(lst.at(1), Qt::MatchExactly))
		ui->m_lpCountry->setCurrentText(lst.at(1));

	if(ui->m_lpCity->findText(lst.at(0), Qt::MatchExactly))
		ui->m_lpCity->setCurrentText(lst.at(0));
}

qint32 cAddLocationDialog::id()
{
	QString		szName			= ui->m_lpName->text();

	QSqlQuery	query(m_dbMySQL);
	QString		szSQL	= QString("SELECT id FROM location WHERE name='%1';").arg(szName);

	if(!query.exec(szSQL))
	{
		qDebug() << query.lastError().text();
		return(-1);
	}
	if(!query.size())
		return(-1);

	if(!query.next())
		return(-1);

	return(query.value("id").toInt());
}
