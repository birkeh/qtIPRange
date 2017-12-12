#ifndef CIPADDRESS_H
#define CIPADDRESS_H


#include <QString>

#include <QMetaType>
#include <QString>
#include <QList>

class cIPRange;

class cIPAddress
{
public:
	cIPAddress();
	cIPAddress(const QString& szIPAddress, const QString& szAddress = QString(""));
	bool			setIPAddress(const QString& szIPAddress);
	bool			setIPAddress(const qint64& iIPAddress);
	QString			IPAddress();
	qint64			IPAddressBin();
	void			setAddress(const QString& szAddress);
	QString			address();
	void			setIPRange(cIPRange* lpIPRange);
	cIPRange*		ipRange();

	static bool		isValid(const QString& szIPAddress);

	static qint64	ip2bin(const QString& szIPAddress);
	static QString	bin2ip(const qint64& iIPAddress);
private:
	qint64			m_iIPAddress;
	QString			m_szAddress;
	cIPRange*		m_lpIPRange;
};

Q_DECLARE_METATYPE(cIPAddress*)

class cIPAddressList : public QList<cIPAddress*>
{
public:
	cIPAddress*		add(const QString& szIPAddress, const QString& szAddress = QString(""));
	void			sort();
};

#endif // CIPADDRESS_H
