#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "ciprange.h"
#include "csplashscreen.h"
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

/*!
 \brief The cMainWindow class provides main routines for this application.

 \class cMainWindow cmainwindow.h "cmainwindow.h"
*/
class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*!
	 \brief Creates a cMainWindow object with the given <i>parent</i> and displays the lists loaded from the database.

	 \fn cMainWindow
	 \param parent
	*/
	explicit cMainWindow(cSplashScreen* lpSplashScreen, QWidget *parent = nullptr);
	/*!
	 \brief Destroys the window.

	 \fn ~cMainWindow
	*/
	~cMainWindow();

private slots:
	/*!
	 \brief This funtion is called when the menu entry "Open File" is triggered.

	 \fn on_m_lpMenuFileOpen_triggered
	*/
	void						on_m_lpMenuFileOpen_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Save File" is triggered.

	 \fn on_m_lpMenuFileSave_triggered
	*/
	void						on_m_lpMenuFileSave_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Save File As..." is triggered.

	 \fn on_m_lpMenuFileSaveAs_triggered
	*/
	void						on_m_lpMenuFileSaveAs_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Load clients from Database" is triggered.

	 \fn on_m_lpMenuFileLoadClientsFromDB_triggered
	*/
	void						on_m_lpMenuFileLoadClientsFromDB_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Reload Location" is triggered.

	 \fn on_m_lpMenuFileReloadLocationList_triggered
	*/
	void						on_m_lpMenuFileReloadLocationList_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Export to SQL" is triggered.

	 \fn on_m_lpMenuFileExportToSQL_triggered
	*/
	void						on_m_lpMenuFileExportToSQL_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Export to Database" is triggered.

	 \fn on_m_lpMenuFileExportToDatabase_triggered
	*/
	void						on_m_lpMenuFileExportToDatabase_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Export to Database" is triggered.

	 \fn on_m_lpMenuFileExportToCSV_triggered
	*/
	void						on_m_lpMenuFileExportToCSV_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Export to Database" is triggered.

	 \fn on_m_lpMenuFileExportToDatabase_triggered
	*/
	void						on_m_lpMenuFileExportToExcel_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Import IP Ranges from Database" is triggered.

	 \fn on_m_lpMenuImportIPRangesFromDB_triggered
	*/
	void						on_m_lpMenuImportIPRangesFromDB_triggered();

	/*!
	 \brief This funtion is called when the menu entry "Verify" is triggered.

	 \fn on_m_lpMenuActionVerify_triggered
	*/
	void						on_m_lpMenuActionVerify_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Find previous error" is triggered.

	 \fn on_m_lpMenuActionFindPreviousError_triggered
	*/
	void						on_m_lpMenuActionFindPreviousError_triggered();
	/*!
	 \brief This funtion is called when the menu entry "find next error" is triggered.

	 \fn on_m_lpMenuActionFindNextError_triggered
	*/
	void						on_m_lpMenuActionFindNextError_triggered();
	/*!
	 \brief This funtion is called when the menu entry "Find" is triggered.

	 \fn on_m_lpMenuActionFind_triggered
	*/
	void						on_m_lpMenuActionFind_triggered();
	/*!
	 \brief This funtion is called when the menu entry "find next" is triggered.

	 \fn on_m_lpMenuActionFindNext_triggered
	*/
	void						on_m_lpMenuActionFindNext_triggered();

	/*!
	 \brief This function is called when the IP range has been changed.

	 \fn ipRangeChanged
	 \param lpIPRange
	 \param lpItem
	*/
	void						ipRangeChanged(cIPRange* lpIPRange, QStandardItem* lpItem);
	/*!
	 \brief This function is called when the location has been changed.

	 \fn locationChanged
	 \param lpIPRange
	 \param lpItem
	*/
	void						locationChanged(cIPRange* lpIPRange, QStandardItem* lpItem);

	/*!
	 \brief This function is called when a context menu has been requested.

	 \fn on_m_lpIPRangeList_customContextMenuRequested
	 \param pos
	*/
	void						on_m_lpIPRangeList_customContextMenuRequested(const QPoint &pos);

	/*!
	 \brief This function is called when a new IP range has been requested.

	 \fn onIPRangeAdd
	*/
	void						onIPRangeAdd();
	/*!
	 \brief This function is called when a new IP range has been requested.

	 \fn onIPRangeCreate
	*/
	void						onIPRangeCreate();
	/*!
	 \brief This function is called when an IP range is requested to be deleted.

	 \fn onIPRangeDelete
	*/
	void						onIPRangeDelete();

	/*!
	 \brief This function is called to select a specific IP range.

	 \fn onGotoRange
	*/
	void						onGotoRange();

	/*!
	 \brief This function is called to add a new location.

	 \fn onLocationAdd
	*/
	void						onLocationAdd();

	/*!
	 \brief This function is called when a context menu has been requested.

	 \fn on_m_lpIPAddressList_customContextMenuRequested
	 \param pos
	*/
	void						on_m_lpIPAddressList_customContextMenuRequested(const QPoint &pos);
	/*!
	 \brief This function is called if the search filter has been changed.

	 \fn on_m_lpFilter_clicked
	*/
	void						on_m_lpFilter_clicked();

	/*!
	 \brief this function is called if the search filter for old locations has been toggled.

	 \fn on_m_lpFilterOldLocation_clicked
	*/
	void						on_m_lpFilterOldLocation_clicked();
	/*!
	 \brief This function is called if the search filter for errors has been toggled.

	 \fn on_m_lpFilterError_clicked
	*/
	void						on_m_lpFilterError_clicked();

	/*!
	 \briefThis function is called if the user switched to another tab.

	 \fn on_m_lpTab_currentChanged
	 \param index
	*/
	void						on_m_lpTab_currentChanged(int index);

private:
	Ui::cMainWindow*			ui;							/*!< This is the user interface of the main window.  */
	QStandardItemModel*			m_lpIPRangeModel;			/*!< This is the item model for the IP range list. */
	QStandardItemModel*			m_lpIPAddressModel;			/*!< This is the item model for the IP address list. */

	QSqlDatabase				m_db;						/*!< This property is used to access the local database where all values are stored. */
	cIPRangeList				m_ipRangeList;				/*!< This is the list of available IP ranges. */
	cLocationList				m_locationList;				/*!< This is the list of available locations. */
	cIPAddressList				m_ipAddressList;			/*!< This is the list of IP addresses to validate. */

	QProgressBar*				m_lpProgressBar;			/*!< This is the progress bar of the main window. */

	QString						m_szIPRangeFindText;		/*!< This is the text to be searched within the IP range list. */
	quint16						m_IPRangeFindFlags;			/*!< This are the flags to narrow the IP range search. */
	QString						m_szIPAddressFindText;		/*!< This is the text to be searched within the IP address list. */
	quint16						m_IPAddressFindFlags;		/*!< This are the flags to narrow the IP address search. */

	QString						m_szHostName;
	QString						m_szDatabaseName;
	QString						m_szUserName;
	QString						m_szPassword;

	/*!
	 \brief Ths function is called to load the list of available locations from the database.

	 \fn loadLocationList
	*/
	void						loadLocationList(cSplashScreen* lpSplashScreen = 0);

	/*!
	 \brief This function is called to load the list of available IP ranges from the database.

	 \fn loadIPRangeList
	*/
	void						loadIPRangeList();
	/*!
	 \brief This function is called to clear and fill the list of IP ranges.

	 \fn displayIPRangeList
	*/
	void						displayIPRangeList();

	/*!
	 \brief This function is called to clear and fill the list of IP addresses.

	 \fn displayIPAddressList
	*/
	void						displayIPAddressList();
	/*!
	 \brief This function is called to verify all IP addresses in the list against the available IP ranges.

	 \fn verifyIPAddressList
	*/
	void						verifyIPAddressList();

	/*!
	 \brief This function is called to save the list of available IP ranges to the database.

	 \fn saveRange
	 \param lpIPRange
	*/
	void						saveRange(cIPRange *lpIPRange);

	/*!
	 \brief This function is called to display only IP ranges that contain errors.

	 \fn filterError
	*/
	void						filterError();

	/*!
	 \brief This function is called to find an IP range according to the parameters provided.

	 \fn findIPRange
	*/
	void						findIPRange();
	/*!
	 \brief

	 \fn findNextIPRange This function is called to find the next IP range according to the parameters provided.
	*/
	void						findNextIPRange();
	/*!
	 \brief This function is called to find an IP address according to the parameters provided.

	 \fn findIPAddress
	*/
	void						findIPAddress();
	/*!
	 \brief This function is called to find the next IP address according to the parameters provided.

	 \fn findNextIPAddress
	*/
	void						findNextIPAddress();

	/*!
	 \brief This function is called to verify an IP range against search parameters.

	 \fn matchIPRange
	 \param lpItem Pointer to the item that has to be verified.
	 \param lpRange Pointer to the IP range for verification.
	 \param flag Search flags to narrow matches.
	 \return TRUE on success, FALSE otherwise.
	*/
	bool						matchIPRange(QStandardItem* lpItem, cIPRange* lpRange, cFindDialogIPRange::FindIn flag);
	/*!
	 \brief This function is called to verify an IP address against search parameters.

	 \fn matchIPAddress
	 \param lpItem Pointer to the item that has to be verified.
	 \param lpIPAddress Pointer to the IP address for verification.
	 \param flag Search flags to narrow matches.
	 \return TRUE on success, FALSE otherwise.
	*/
	bool						matchIPAddress(QStandardItem* lpItem, cIPAddress* lpIPAddress, cFindDialogIPAddress::FindIn flag);

protected:
	/*!
	 \brief

	 \fn closeEvent
	 \param event
	*/
	void						closeEvent(QCloseEvent* event);
};

#endif // CMAINWINDOW_H
