#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "ciprange.h"
#include "clocation.h"

#include "ciprangeitemdelegate.h"

#include <QStandardItemModel>
#include <QMainWindow>

#include <QSqlDatabase>


namespace Ui {
class cMainWindow;
}

class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit cMainWindow(QWidget *parent = 0);
	~cMainWindow();

private slots:
	void					on_m_lpMenuFileOpen_triggered();
	void					on_m_lpMenuFileSave_triggered();
	void					on_m_lpMenuFileSaveAs_triggered();

private:
	Ui::cMainWindow*		ui;
	QStandardItemModel*		m_lpIPRangeModel;

	QSqlDatabase			m_db;
	cIPRangeList			m_ipRangeList;
	cLocationList			m_locationList;

	void					loadLocationList();

	void					loadIPRangeList();
	void					displayIPRangeList();
};

#endif // CMAINWINDOW_H
