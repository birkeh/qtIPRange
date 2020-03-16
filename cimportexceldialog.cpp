#include "cimportexceldialog.h"
#include "ui_cimportexceldialog.h"

#include <QSettings>
#include <QFileDialog>

#include <QDebug>


cImportExcelDialog::cImportExcelDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cImportExcelDialog),
	m_lpExcel(nullptr),
	m_lpExcelImportItemDelegate(nullptr)
{
	ui->setupUi(this);

	m_lpFieldListModel	= new QStandardItemModel;
	ui->m_lpFieldList->setModel(m_lpFieldListModel);
	ui->m_lpFieldList->setItemDelegate(new cImportExcelItemDelegate());

	m_lpDataListModel	= new QStandardItemModel;
	ui->m_lpDataList->setModel(m_lpDataListModel);
}

cImportExcelDialog::~cImportExcelDialog()
{
	if(m_lpExcel)
		delete m_lpExcel;

	delete ui;
}

void cImportExcelDialog::on_m_lpFileNameBrowse_clicked()
{
	QSettings	settings;
	QString		fileName	= QFileDialog::getOpenFileName(this, tr("Open Excel"), settings.value("import/lastExcelImport").toString(), tr("Excel Files (*.xlsx)"));

	if(fileName.isEmpty())
		return;

	QDir		dir(fileName);
	settings.setValue("import/lastExcelImport", QVariant::fromValue(dir.path()));

	ui->m_lpFileName->setText(fileName);
}

void cImportExcelDialog::on_m_lpFileName_textChanged(const QString &arg1)
{
	if(arg1.length())
	{
		ui->m_lpOpen->setEnabled(true);
		ui->m_lpTest->setEnabled(true);
	}
	else
	{
		ui->m_lpOpen->setEnabled(false);
		ui->m_lpTest->setEnabled(false);
		m_lpFieldListModel->clear();
		m_lpDataListModel->clear();
	}
}

void cImportExcelDialog::on_m_lpOpen_clicked()
{
	if(ui->m_lpFileName->text().isEmpty())
		return;

	if(m_lpExcel)
		delete m_lpExcel;

	m_lpExcel	= new QXlsx::Document(ui->m_lpFileName->text());
	ui->m_lpSheet->clear();

	m_lpFieldListModel->clear();
	m_lpDataListModel->clear();

	ui->m_lpSheet->addItems(m_lpExcel->sheetNames());
	ui->m_lpSheet->setCurrentIndex(0);
}

void cImportExcelDialog::on_m_lpSheet_currentIndexChanged(const QString &arg1)
{
	if(!m_lpExcel)
		return;

	if(!m_lpExcel->selectSheet(arg1))
		return;

	m_lpFieldListModel->clear();

	QStringList	header;

	header << tr("Import Field") << tr("Internal Field");
	m_lpFieldListModel->setHorizontalHeaderLabels(header);

	QStringList	headerList;

	for(int x = 0;;x++)
	{
		QString	field	= m_lpExcel->read(1, x+1).toString();
		if(field.isEmpty())
			break;

		QList<QStandardItem*>	itemList;

		itemList.append(new QStandardItem(field));
		itemList.append(new QStandardItem(""));

		m_lpFieldListModel->appendRow(itemList);
	}
}

void cImportExcelDialog::on_m_lpTest_clicked()
{

}
