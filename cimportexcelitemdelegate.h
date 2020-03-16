#ifndef CIMPORTEXCELITEMDELEGATE_H
#define CIMPORTEXCELITEMDELEGATE_H


#include <QStyledItemDelegate>
#include <QStandardItem>


class cImportExcelItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	cImportExcelItemDelegate(QObject* parent = 0);
	~cImportExcelItemDelegate();

	virtual QWidget*	createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void		setEditorData(QWidget *editor, const QModelIndex &index) const;
//	virtual void		setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:
};

#endif // CIMPORTEXCELITEMDELEGATE_H
