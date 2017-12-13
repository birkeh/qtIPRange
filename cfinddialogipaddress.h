#ifndef CFINDDIALOGIPADDRESS_H
#define CFINDDIALOGIPADDRESS_H


#include <QDialog>


namespace Ui {
class cFindDialogIPAddress;
}

class cFindDialogIPAddress : public QDialog
{
	Q_OBJECT

public:
	enum FindIn
	{
		FindInIPAddress		= 0x0001,
		FindInRange			= 0x0002,
		FindInOriLocation	= 0x0004,
		FindInLocation		= 0x0008,
		FindInAll			= 0x000F,
	};

	explicit cFindDialogIPAddress(QWidget *parent = 0);
	~cFindDialogIPAddress();

	void			setFindText(const QString& szFindText);
	QString			findText();

	void			setFindFlags(const quint16& flags);
	quint16			findFlags();

private slots:
	void			on_m_lpFindIn_clicked();
	void			on_m_lpFindText_textChanged(const QString &arg1);

private:
	Ui::cFindDialogIPAddress *ui;
};

#endif // CFINDDIALOGIPADDRESS_H
