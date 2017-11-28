#ifndef CIPRANGE_H
#define CIPRANGE_H


#include "cipaddress.h"

#include <QMetaType>
#include <QString>
#include <QList>
#include <QStandardItemModel>


class cIPRangeList;

class cIPRange
{
public:
	cIPRange();
	cIPRange(const QString& szIPRange);

	bool			setIPRange(const QString& szIPRange);
	QString			firstIPAddress();
	QString			lastIPAddress();
	qint64			firstIPAddressBin();
	qint64			lastIPAddressBin();
	QString			broadcastIPAddress();
	qint64			broadcastIPAddressBIN();
	QString			netmask();
	QString			range();

	void			setName(QString& szName);
	void			setBaseRange(QString& szIPRange);

	cIPAddress		IPAddress();
	qint16			prefix();

	QString			name();
	cIPAddress		IPAddressBase();
	qint16			prefixBase();
	QString			rangeBase();

private:
	cIPAddress		m_IPAddress;
	qint16			m_iPrefix;
	QString			m_szName;
	cIPAddress		m_IPAddressBase;
	qint16			m_iPrefixBase;

	qint32			netmaskBin();
};

Q_DECLARE_METATYPE(cIPRange*)

class cIPRangeList : public QList<cIPRange*>
{
public:
	cIPRange*		add(const QString& szIPRange);

	bool			load(const QString& szFilename);
	void			clean();

	bool			fillList(QStandardItemModel* lpModel);
};

#endif // CIPRANGE_H
