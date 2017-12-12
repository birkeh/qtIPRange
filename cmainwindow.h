#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "ciprange.h"
#include "clocation.h"

#include "ciprangeitemdelegate.h"

#include <QStandardItemModel>
#include <QMainWindow>

#include <QSqlDatabase>

#include <QProgressBar>


namespace Ui {
class cMainWindow;
}

class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit cMainWindow(QWidget *parent = 0);
	~cMainWindow();

private slots:
	void					on_m_lpMenuFileOpen_triggered();
	void					on_m_lpMenuFileSave_triggered();
	void					on_m_lpMenuFileSaveAs_triggered();

	void					ipRangeChanged(cIPRange* lpIPRange, QStandardItem* lpItem);
	void					locationChanged(cIPRange* lpIPRange, QStandardItem* lpItem);
	void					on_m_lpMenuQctionVerify_triggered();

	void					on_m_lpIPRangeList_customContextMenuRequested(const QPoint &pos);

	void					onIPRangeAdd();
	void					onIPRangeCreate();
	void					onIPRangeDelete();
	void					on_actionLoad_clients_from_DB_triggered();

	void					on_m_lpIPAddressList_customContextMenuRequested(const QPoint &pos);
	void					on_actionReload_location_list_triggered();
	void					on_m_lpFilter_clicked();

	void					on_m_lpFilterError_clicked();
	void					on_m_lpFilterOldLocation_clicked();

	void on_actionto_SQL_file_triggered();

	void on_actionto_database_triggered();

	void on_actionIP_ranges_from_DB_triggered();

private:
	Ui::cMainWindow*		ui;
	QStandardItemModel*		m_lpIPRangeModel;
	QStandardItemModel*		m_lpIPAddressModel;

	QSqlDatabase			m_db;
	cIPRangeList			m_ipRangeList;
	cLocationList			m_locationList;
	cIPAddressList			m_ipAddressList;

	QProgressBar*			m_lpProgressBar;

	void					loadLocationList();

	void					loadIPRangeList();
	void					displayIPRangeList();

	void					displayIPAddressList();
	void					verifyIPAddressList();

	void					saveRange(cIPRange *lpIPRange);

	void					filterError();
};

#endif // CMAINWINDOW_H
