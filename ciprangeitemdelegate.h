#ifndef CIPRANGEITEMDELEGATE_H
#define CIPRANGEITEMDELEGATE_H


#include <QStyledItemDelegate>


class cIPRangeItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	cIPRangeItemDelegate(QObject* parent = 0);
	~cIPRangeItemDelegate();

	virtual QWidget*	createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void		setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void		setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // CIPRANGEITEMDELEGATE_H
