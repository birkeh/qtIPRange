#ifndef CIPRANGE_H
#define CIPRANGE_H


#include "cipaddress.h"

#include <QMetaType>
#include <QString>
#include <QList>


class cIPRangeList;

class cIPRange
{
public:
	cIPRange();
	cIPRange(const QString& szIPRange);

	static bool		isValid(const QString& szIPRange);

	bool			setIPRange(const QString& szIPRange);
	QString			firstIPAddress();
	QString			lastIPAddress();
	qint64			firstIPAddressBin();
	qint64			lastIPAddressBin();
	QString			broadcastIPAddress();
	qint64			broadcastIPAddressBIN();
	QString			netmask();
	quint64			netmaskBin();
	QString			range();

	void			setOK(const bool &bOK);
	bool			ok();

	void			setName(QString& szName);

	cIPAddress		IPAddress();
	qint16			prefix();

	QString			name();
	QString			oldName();

	void			setLocation(const qint32 iLocation);
	qint32			location();

	bool			ipInRange(const QString& szIPAddress);
	bool			ipInRange(const qint64& iIPAddress);

	qint64			subnetBIN();
	qint64			subnet1BIN();
	qint64			subnet2BIN();
	qint64			subnet3BIN();
private:
	cIPAddress		m_IPAddress;
	qint16			m_iPrefix;
	QString			m_szName;
	QString			m_szOldName;
	qint32			m_iLocation;
	bool			m_bOK;
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

	qint32			position(cIPRange* lpIPRange);
};

#endif // CIPRANGE_H
