#ifndef CADDCITYDIALOG_H
#define CADDCITYDIALOG_H

#include <QDialog>

namespace Ui {
class cAddCityDialog;
}

class cAddCityDialog : public QDialog
{
	Q_OBJECT

public:
	explicit cAddCityDialog(QWidget *parent = 0);
	~cAddCityDialog();

	QString		city();
	qint32		postalCode();
private slots:
	void on_m_lpCity_textChanged(const QString &arg1);

private:
	Ui::cAddCityDialog *ui;
};

#endif // CADDCITYDIALOG_H
