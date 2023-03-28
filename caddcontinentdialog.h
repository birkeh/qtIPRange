#ifndef CADDCITYDIALOG_H
#define CADDCITYDIALOG_H

#include <QDialog>

namespace Ui {
class cAddCityDialog;
}

/*!
 \brief The cAddCityDialog class provides a dialog window to add new city values to the database.

With the constructor, the parent of this instance may be set.

	cAddCityDialog* lpDialog = new cAddCityDialog(this);
	lpDialog->exec();

	QString szCity = lpDialog->city();
	qint32 postalCode = lpDialog->postalCode();

	delete lpDialog;

 \class cAddCityDialog caddcitydialog.h "caddcitydialog.h"
*/
class cAddCityDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 \brief Creates a cAddCityDialog with the given <i>parent</i> to enter new city values.

	 \fn cAddCityDialog
	 \param parent
	*/
	explicit cAddCityDialog(QWidget *parent = 0);
	/*!
	 \brief Destroys the dialog.

	 \fn ~cAddCityDialog
	*/
	~cAddCityDialog();

	/*!
	 \brief

	 \fn city
	 \return Returns the name of the new city.
	*/
	QString		city();
	/*!
	 \brief

	 \fn postalCode
	 \return Returns the postal code of the new city.
	*/
	qint32		postalCode();
private slots:
	/*!
	 \brief This function is called when the name of the city has been changed.

	 \fn on_m_lpCity_textChanged
	 \param arg1 New name of the city.
	*/
	void on_m_lpCity_textChanged(const QString &arg1);

private:
	Ui::cAddCityDialog *ui; /*!< This is the user interface of the dialog.  */
};

#endif // CADDCITYDIALOG_H
