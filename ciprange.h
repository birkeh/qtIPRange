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

	void			setOK(const bool &bOK);
	bool			ok();

	void			setName(QString& szName);
	void			setBaseRange(QString& szIPRange);

	cIPAddress		IPAddress();
	qint16			prefix();

	QString			name();
	cIPAddress		IPAddressBase();
	qint16			prefixBase();
	QString			rangeBase();

	bool			ipInRange(const QString& szIPAddress);
	bool			ipInRange(const qint64& iIPAddress);
private:
	cIPAddress		m_IPAddress;
	qint16			m_iPrefix;
	QString			m_szName;
	cIPAddress		m_IPAddressBase;
	qint16			m_iPrefixBase;
	bool			m_bOK;
	qint32			netmaskBin();
};

Q_DECLARE_METATYPE(cIPRange*)

class cIPRangeList : public QList<cIPRange*>
{
public:
	cIPRange*		add(const QString& szIPRange);
	void			clean();
	void			sort();
	void			verify();

	cIPRange*		findRange(const QString& szIPAddress);
	cIPRange*		findRange(const qint64& iIPAddress);
};

#endif // CIPRANGE_H
