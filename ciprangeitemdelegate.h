#ifndef CIPRANGEITEMDELEGATE_H
#define CIPRANGEITEMDELEGATE_H


#include "ciprange.h"

#include <QStyledItemDelegate>
#include <QStandardItem>


class cIPRangeItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	cIPRangeItemDelegate(QObject* parent = 0);
	~cIPRangeItemDelegate();

	virtual QWidget*	createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void		setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void		setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:
	void				ipRangeChanged(cIPRange* lpIPRange, QStandardItem* lpItem) const;
};

#endif // CIPRANGEITEMDELEGATE_H
