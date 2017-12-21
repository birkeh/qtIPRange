#ifndef CADDLOCATIONDIALOG_H
#define CADDLOCATIONDIALOG_H


#include <QDialog>
#include <QSqlDatabase>


namespace Ui {
class cAddLocationDialog;
}

class cAddLocationDialog : public QDialog
{
	Q_OBJECT

public:
	explicit cAddLocationDialog(QWidget *parent = 0);
	~cAddLocationDialog();

	void				setLocation(const QString& szLocation);
	qint32				id();
private slots:
	void				on_m_lpAddCountry_clicked();
	void				on_m_lpAddCity_clicked();
	void				on_m_lpName_textChanged(const QString &arg1);
	void				on_m_lpLocation_textChanged(const QString &arg1);

private:
	Ui::cAddLocationDialog *ui;
	QSqlDatabase		m_dbMySQL;

	void				loadCountryList();
	void				loadFederalStateList();
	void				loadCityList();

	void				accept();
};

#endif // CADDLOCATIONDIALOG_H
