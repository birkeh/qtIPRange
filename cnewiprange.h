#ifndef CNEWIPRANGE_H
#define CNEWIPRANGE_H


#include "clocation.h"

#include <QDialog>


namespace Ui {
class cNewIPRange;
}

/*!
 \brief The cNewIPRange class provides a dialog window to add new IP ranges to the database.

 \class cNewIPRange cnewiprange.h "cnewiprange.h"
*/
class cNewIPRange : public QDialog
{
	Q_OBJECT

public:
	/*!
	 \brief Creates a cNewIPRange window with the given <i>parent</i>.

	 \fn cNewIPRange
	 \param parent
	*/
	explicit cNewIPRange(QWidget *parent = 0);
	/*!
	 \brief Destroys the dialog.

	 \fn ~cNewIPRange
	*/
	~cNewIPRange();

	/*!
	 \brief Sets the values within the dialog.

	 \fn setIPRange
	 \param szIPRange IP range in textual format (x.x.x.x/x).
	*/
	void			setIPRange(const QString& szIPRange);
	/*!
	 \brief Returns the IP range in textual format (x.x.x.x/x).

	 \fn ipRange
	 \return IP range in textual format (x.x.x.x/x).
	*/
	QString			ipRange();

	/*!
	 \brief Selects the entry within the location list.

	 \fn setLocation
	 \param locationList List of available locations.
	 \param szLocation Name of the location to be selected.
	*/
	void			setLocation(const cLocationList& locationList, const QString& szLocation = QString(""));
	/*!
	 \brief Returns the selected location name within the list.

	 \fn location
	 \return Index of the selected location.
	*/
	qint32			location();
private:
	Ui::cNewIPRange *ui; /*!< This is the user interface of the dialog.  */
};

#endif // CNEWIPRANGE_H
