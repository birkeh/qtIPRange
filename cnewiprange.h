#ifndef CNEWIPRANGE_H
#define CNEWIPRANGE_H


#include "clocation.h"

#include <QDialog>


namespace Ui {
class cNewIPRange;
}

class cNewIPRange : public QDialog
{
	Q_OBJECT

public:
	explicit cNewIPRange(QWidget *parent = 0);
	~cNewIPRange();

	void			setIPRange(const QString& szIPRange);
	QString			ipRange();

	void			setLocation(const cLocationList& locationList);
	qint32			location();
private:
	Ui::cNewIPRange *ui;
};

#endif // CNEWIPRANGE_H
