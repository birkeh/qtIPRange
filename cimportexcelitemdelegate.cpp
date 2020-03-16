#include "cimportexcelitemdelegate.h"

#include <QComboBox>


cImportExcelItemDelegate::cImportExcelItemDelegate(QObject* parent) :
	QStyledItemDelegate(parent)
{
}

cImportExcelItemDelegate::~cImportExcelItemDelegate()
{
}

QWidget* cImportExcelItemDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	if(index.column() == 0)
		return(0);

	QComboBox*		lpComboBox		= new QComboBox(parent);
	QStringList		items;

	items << " " << "Range" << "IP Address" << "Subnet" << "Prefix" << "Mask" << "Location";
	lpComboBox->addItems(items);

	return(lpComboBox);
}

void cImportExcelItemDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
	QComboBox*	lpComboBox	= qobject_cast<QComboBox*>(editor);

	if(lpComboBox)
	{
		QString	currentText	= index.data(Qt::EditRole).toString();
		int		cbIndex		= lpComboBox->findText(currentText);

		if(cbIndex >= 0)
			lpComboBox->setCurrentIndex(cbIndex);
	}
	else
	{
		QStyledItemDelegate::setEditorData(editor, index);
	}
}

//void cImportExcelItemDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
//{
//	QComboBox*	lpComboBox	= qobject_cast<QComboBox*>(editor);
//	QLineEdit*	lpLineEdit	= qobject_cast<QLineEdit*>(editor);

//	if(lpComboBox)
//	{
//		model->setData(index, lpComboBox->currentText(), Qt::EditRole);
//		QModelIndex	index1		= model->index(index.row(), 0);
//		cIPRange*	lpIPRange	= qvariant_cast<cIPRange*>(index1.data(Qt::UserRole));

//		if(lpIPRange->location() != lpComboBox->currentData().toInt())
//		{
//			QStandardItemModel*	lpModel		= (QStandardItemModel*)index.model();
//			QStandardItem*		lpItem		= lpModel->itemFromIndex(index1);

//			lpIPRange->setLocation(lpComboBox->currentData().toInt());
//			locationChanged(lpIPRange, lpItem);
//		}
//	}
//	else if(lpLineEdit)
//	{
//		QString				strText		= lpLineEdit->text();

//		if(!cIPRange::isValid(strText))
//			return;

//		cIPRange*			lpIPRange	= qvariant_cast<cIPRange*>(index.data(Qt::UserRole));
//		QStandardItemModel*	lpModel		= (QStandardItemModel*)index.model();
//		QStandardItem*		lpItem		= lpModel->itemFromIndex(index);

//		lpIPRange->setIPRange(strText);
//		QStyledItemDelegate::setModelData(editor, model, index);
//		emit ipRangeChanged(lpIPRange, lpItem);
//	}
//}
