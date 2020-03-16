#ifndef CIMPORTEXCELDIALOG_H
#define CIMPORTEXCELDIALOG_H


#include "xlsxdocument.h"

#include "cimportexcelitemdelegate.h"

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
class cImportExcelDialog;
}

class cImportExcelDialog : public QDialog
{
	Q_OBJECT

public:
	explicit cImportExcelDialog(QWidget *parent = nullptr);
	~cImportExcelDialog();

private slots:
	void on_m_lpFileNameBrowse_clicked();
	void on_m_lpFileName_textChanged(const QString &arg1);
	void on_m_lpOpen_clicked();
	void on_m_lpSheet_currentIndexChanged(const QString &arg1);

	void on_m_lpTest_clicked();

private:
	Ui::cImportExcelDialog*		ui;
	QXlsx::Document*			m_lpExcel;

	QStandardItemModel*			m_lpFieldListModel;
	QStandardItemModel*			m_lpDataListModel;
	cImportExcelItemDelegate*	m_lpExcelImportItemDelegate;
};

#endif // CIMPORTEXCELDIALOG_H
