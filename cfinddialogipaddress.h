#ifndef CFINDDIALOGIPADDRESS_H
#define CFINDDIALOGIPADDRESS_H


#include <QDialog>


namespace Ui {
class cFindDialogIPAddress;
}

/*!
 \brief The cFindDialogIPAddress class provides a dialog window to search for IP addresses.

The search may be narrowed by following flags:

<table>
<tr><td>FindInIPAddress</td><td>0x0001</td><td>search in IP addresses</td></tr>
<tr><td>FindInRange</td><td>0x0002</td><td>search in IP ranges</td></tr>
<tr><td>FindInOriLocation</td><td>0x0004</td><td>search in original locations</td></tr>
<tr><td>FindInLocation</td><td>0x0008</td><td>search in locations</td></tr>
<tr><td>FindInAll</td><td>0x000F</td><td>search in all fields</td></tr>
</table>

The search is executed in the list of IP addresses.

 \class cFindDialogIPAddress cfinddialogipaddress.h "cfinddialogipaddress.h"
*/
class cFindDialogIPAddress : public QDialog
{
	Q_OBJECT

public:
	/*!
	 \brief Search flags to narrow the search.

	 \enum FindIn
	*/
	enum FindIn
	{
		FindInIPAddress		= 0x0001, /*!< search in IP addresses */
		FindInRange			= 0x0002, /*!< search in IP ranges */
		FindInOriLocation	= 0x0004, /*!< search in original locations */
		FindInLocation		= 0x0008, /*!< search in locations */
		FindInAll			= 0x000F, /*!< search in all fields */
	};

	/*!
	 \brief Creates a cFindDialogIPAddress with the given <i>parent</i>.

	 \fn cFindDialogIPAddress
	 \param parent
	*/
	explicit cFindDialogIPAddress(QWidget *parent = 0);
	/*!
	 \brief Destroys the dialog.

	 \fn ~cFindDialogIPAddress
	*/
	~cFindDialogIPAddress();

	/*!
	 \brief Sets the text for the search.

	 \fn setFindText
	 \param szFindText text to search for.
	*/
	void			setFindText(const QString& szFindText);
	/*!
	 \brief Returns the search test.

	 \fn findText
	 \return Value of the text to search for
	*/
	QString			findText();

	/*!
	 \brief Sets the flags to narrow the search. The default value is to search in all fields.

	 \fn setFindFlags
	 \param flags Flags to narrow the search
	*/
	void			setFindFlags(const quint16& flags);
	/*!
	 \brief Returns the flags of the narrowed search.

	 \fn findFlags
	 \return Flags
	*/
	quint16			findFlags();

private slots:
	/*!
	 \brief This function is called when one of the flags buttons is triggered.

	 \fn on_m_lpFindIn_clicked
	*/
	void			on_m_lpFindIn_clicked();
	/*!
	 \brief This function is called when the search test has been changed.

	 \fn on_m_lpFindText_textChanged
	 \param arg1 New search test.
	*/
	void			on_m_lpFindText_textChanged(const QString &arg1);

private:
	Ui::cFindDialogIPAddress *ui; /*!< This is the user interface of the search fialog. */
};

#endif // CFINDDIALOGIPADDRESS_H
