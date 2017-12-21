#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "ciprange.h"
#include "clocation.h"

#include "ciprangeitemdelegate.h"

#include "cfinddialogiprange.h"
#include "cfinddialogipaddress.h"

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
	void						on_m_lpMenuFileOpen_triggered();
	void						on_m_lpMenuFileSave_triggered();
	void						on_m_lpMenuFileSaveAs_triggered();
	void						on_m_lpMenuFileLoadClientsFromDB_triggered();
	void						on_m_lpMenuFileReloadLocationList_triggered();
	void						on_m_lpMenuFileExportToSQL_triggered();
	void						on_m_lpMenuFileExportToDatabase_triggered();
	void						on_m_lpMenuImportIPRangesFromDB_triggered();

	void						on_m_lpMenuActionVerify_triggered();
	void						on_m_lpMenuActionFindPreviousError_triggered();
	void						on_m_lpMenuActionFindNextError_triggered();
	void						on_m_lpMenuActionFind_triggered();
	void						on_m_lpMenuActionFindNext_triggered();

	void						ipRangeChanged(cIPRange* lpIPRange, QStandardItem* lpItem);
	void						locationChanged(cIPRange* lpIPRange, QStandardItem* lpItem);

	void						on_m_lpIPRangeList_customContextMenuRequested(const QPoint &pos);

	void						onIPRangeAdd();
	void						onIPRangeCreate();
	void						onIPRangeDelete();

	void						onGotoRange();

	void						onLocationAdd();

	void						on_m_lpIPAddressList_customContextMenuRequested(const QPoint &pos);
	void						on_m_lpFilter_clicked();

	void						on_m_lpFilterOldLocation_clicked();
	void						on_m_lpFilterError_clicked();

	void						on_m_lpTab_currentChanged(int index);

private:
	Ui::cMainWindow*			ui;
	QStandardItemModel*			m_lpIPRangeModel;
	QStandardItemModel*			m_lpIPAddressModel;

	QSqlDatabase				m_db;
	cIPRangeList				m_ipRangeList;
	cLocationList				m_locationList;
	cIPAddressList				m_ipAddressList;

	QProgressBar*				m_lpProgressBar;

	QString						m_szIPRangeFindText;
	quint16						m_IPRangeFindFlags;
	QString						m_szIPAddressFindText;
	quint16						m_IPAddressFindFlags;

	void						loadLocationList();

	void						loadIPRangeList();
	void						displayIPRangeList();

	void						displayIPAddressList();
	void						verifyIPAddressList();

	void						saveRange(cIPRange *lpIPRange);

	void						filterError();

	void						findIPRange();
	void						findNextIPRange();
	void						findIPAddress();
	void						findNextIPAddress();

	bool						matchIPRange(QStandardItem* lpItem, cIPRange* lpRange, cFindDialogIPRange::FindIn flag);
	bool						matchIPAddress(QStandardItem* lpItem, cIPAddress* lpIPAddress, cFindDialogIPAddress::FindIn flag);
};

#endif // CMAINWINDOW_H
