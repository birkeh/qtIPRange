#ifndef CFINDDIALOGIPRANGE_H
#define CFINDDIALOGIPRANGE_H


#include <QDialog>


namespace Ui {
class cFindDialogIPRange;
}

/*!
 \brief The cFindDialogIPRange class provides a dialog window to search for IP addresses.

The search may be narrowed by following flags:

<table>
<tr><td>FindInName</td><td>0x0001</td><td>search in IP range names </td></tr>
<tr><td>FindInRange</td><td>0x0002</td><td>search in IP ranges </td></tr>
<tr><td>FindInBaseIP</td><td>0x0004</td><td>search in base IP addresses </td></tr>
<tr><td>FindInPrefix</td><td>0x0008</td><td>search in IP prefixes </td></tr>
<tr><td>FindInSubnetMask</td><td>0x0010</td><td>search in subnet masks </td></tr>
<tr><td>FindInBroadcastIP</td><td>0x0020</td><td>search in broadcast IP addresses </td></tr>
<tr><td>FindInFirstIP</td><td>0x0040</td><td>search in first IP addresses of ranges </td></tr>
<tr><td>FindInLastIP</td><td>0x0080</td><td>search in last IP addresses of ranges </td></tr>
<tr><td>FindInLocation</td><td>0x0100</td><td>search in location names </td></tr>
<tr><td>FindInAll</td><td>0x01FF</td><td>search in all fields </td></tr>
</table>

The search is executed in the list of IP ranges.

 \class cFindDialogIPRange cfinddialogiprange.h "cfinddialogiprange.h"
*/
class cFindDialogIPRange : public QDialog
{
	Q_OBJECT

public:
	/*!
	 \brief

	 \enum FindIn
	*/
	enum FindIn
	{
		FindInName			= 0x0001, /*!< search in IP range names */
		FindInRange			= 0x0002, /*!< search in IP ranges */
		FindInBaseIP		= 0x0004, /*!< search in base IP addresses */
		FindInPrefix		= 0x0008, /*!< search in IP prefixes */
		FindInSubnetMask	= 0x0010, /*!< search in subnet masks */
		FindInBroadcastIP	= 0x0020, /*!< search in broadcast IP addresses */
		FindInFirstIP		= 0x0040, /*!< search in first IP addresses of ranges */
		FindInLastIP		= 0x0080, /*!< search in last IP addresses of ranges */
		FindInLocation		= 0x0100, /*!< search in location names */
		FindInAll			= 0x01FF, /*!< search in all fields */
	};

	/*!
	 \brief Creates a cFindDialogIPRange with the given <i>parent</i>.

	 \fn cFindDialogIPRange
	 \param parent
	*/
	explicit cFindDialogIPRange(QWidget *parent = 0);
	/*!
	 \brief Destroys the dialog.

	 \fn ~cFindDialogIPRange
	*/
	~cFindDialogIPRange();

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
	Ui::cFindDialogIPRange *ui; /*!< This is the user interface of the search fialog. */
};

#endif // CFINDDIALOGIPRANGE_H
