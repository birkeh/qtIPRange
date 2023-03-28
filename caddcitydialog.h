#ifndef CADDLOCATIONDIALOG_H
#define CADDLOCATIONDIALOG_H


#include <QDialog>
#include <QSqlDatabase>


namespace Ui {
class cAddLocationDialog;
}

/*!
 \brief The cAddLocationDialog class provides a dialog window to add new location values to the database.

With the constructor, the parent of this instance may be set.

	cAddLocationDialog* lpDialog = new cAddLocationDialog(this);
	lpDialog->exec();

	qint32 id = lpDialog->id();

	delete lpDialog;

 \class cAddLocationDialog caddlocationdialog.h "caddlocationdialog.h"
*/
class cAddCityDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 \brief Creates a cAddLocationDialog with the given <i>parent</i> to enter new location values.

	 \fn cAddLocationDialog
	 \param parent
	*/
	explicit cAddCityDialog(QWidget *parent = 0);
	/*!
	 \brief Destroys the dialog.

	 \fn ~cAddLocationDialog
	*/
	~cAddCityDialog();

	/*!
	 \brief Sets the source location. This value is meant as the default value for the new location.

	 \fn setLocation
	 \param szLocation Name of the location.
	*/
	void				setLocation(const QString& szLocation);
	/*!
	 \brief Returns the id of the newly created location. If the location already exists, the returned value is the value of the existing location.

	 \fn id
	 \return qint32 id of the newly created location.
	*/
	qint32				id();
private slots:
	/*!
	 \brief This function is called when the button "Add Country" has been triggered.

	 \fn on_m_lpAddCountry_clicked
	*/
	void				on_m_lpAddCountry_clicked();
	/*!
	 \brief This function is called when the button "Add City" has been triggered.

	 \fn on_m_lpAddCity_clicked
	*/
	void				on_m_lpAddCity_clicked();
	/*!
	 \brief This function is called when the name of the location has been changed.

	 \fn on_m_lpName_textChanged
	 \param arg1 New name of the location.
	*/
	void				on_m_lpName_textChanged(const QString &arg1);
	/*!
	 \brief This function is called when the location has been changed.

	 \fn on_m_lpLocation_textChanged
	 \param arg1 New location.
	*/
	void				on_m_lpLocation_textChanged(const QString &arg1);

private:
	Ui::cAddLocationDialog *ui;		/*!< This is the user interface of the dialog. */
	QSqlDatabase		m_dbMySQL;	/*!< This is the database that holds the IP range values. */

	/*!
	 \brief This function loads the list of the available countries from the database.

	 \fn loadCountryList
	*/
	void				loadCountryList();
	/*!
	 \brief This function loads the list of the available federal states from the database.

	 \fn loadFederalStateList
	*/
	void				loadFederalStateList();
	/*!
	 \brief This function loads the list of the available cities from the database.

	 \fn loadCityList
	*/
	void				loadCityList();

	/*!
	 \brief Reimplemented from QDialog.

	 \fn accept
	*/
	void				accept();
};

#endif // CADDLOCATIONDIALOG_H
