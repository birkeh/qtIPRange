#ifndef CFINDDIALOGIPRANGE_H
#define CFINDDIALOGIPRANGE_H


#include <QDialog>


namespace Ui {
class cFindDialogIPRange;
}

class cFindDialogIPRange : public QDialog
{
	Q_OBJECT

public:
	enum FindIn
	{
		FindInName			= 0x0001,
		FindInRange			= 0x0002,
		FindInBaseIP		= 0x0004,
		FindInPrefix		= 0x0008,
		FindInSubnetMask	= 0x0010,
		FindInBroadcastIP	= 0x0020,
		FindInFirstIP		= 0x0040,
		FindInLastIP		= 0x0080,
		FindInLocation		= 0x0100,
		FindInAll			= 0x01FF,
	};

	explicit cFindDialogIPRange(QWidget *parent = 0);
	~cFindDialogIPRange();

	void			setFindText(const QString& szFindText);
	QString			findText();

	void			setFindFlags(const quint16& flags);
	quint16			findFlags();

private slots:
	void			on_m_lpFindIn_clicked();
	void			on_m_lpFindText_textChanged(const QString &arg1);

private:
	Ui::cFindDialogIPRange *ui;
};

#endif // CFINDDIALOGIPRANGE_H
