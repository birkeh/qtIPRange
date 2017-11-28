#ifndef CIPADDRESS_H
#define CIPADDRESS_H


#include <QString>


class cIPAddress
{
public:
	cIPAddress();
	cIPAddress(const QString& szIPAddress);
	bool			setIPAddress(const QString& szIPAddress);
	bool			setIPAddress(const qint64& iIPAddress);
	QString			IPAddress();
	qint64			IPAddressBin();

	static bool		isValid(const QString& szIPAddress);

	static qint64	ip2bin(const QString& szIPAddress);
	static QString	bin2ip(const qint64& iIPAddress);
private:
	qint64		m_iIPAddress;
};

#endif // CIPADDRESS_H
