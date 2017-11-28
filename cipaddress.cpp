#include "cipaddress.h"

#include <QStringList>


cIPAddress::cIPAddress()
{
	setIPAddress("0.0.0.0");
}

cIPAddress::cIPAddress(const QString& szIPAddress)
{
	setIPAddress(szIPAddress);
}

bool cIPAddress::setIPAddress(const QString& szIPAddress)
{
	if(!isValid(szIPAddress))
		return(false);

	m_iIPAddress	= ip2bin(szIPAddress);

	return(true);
}

bool cIPAddress::setIPAddress(const qint64& iIPAddress)
{
	m_iIPAddress	= iIPAddress;
}

bool cIPAddress::isValid(const QString& szIPAddress)
{
	QStringList	szList	= szIPAddress.split(".");

	if(szList.count() != 4)
		return(false);

	qint64	ip;

	for(int x = 0;x < 4;x++)
	{
		ip	= szList[0].toLong();
		if(ip < 0 || ip > 255)
			return(false);
	}

	return(true);
}

QString cIPAddress::IPAddress()
{
	return(bin2ip(m_iIPAddress));
}

qint64 cIPAddress::IPAddressBin()
{
	return(m_iIPAddress);
}

qint64 cIPAddress::ip2bin(const QString& szIPAddress)
{
	qint64	ip	= 0;
	qint64	ipHelper;

	QStringList	szList	= szIPAddress.split(".");

	for(int x = 0;x < 4;x++)
	{
		ipHelper	= szList[x].toLong();
		ip	+= ipHelper << (8*(3-x));
	}

	return(ip);
}

QString cIPAddress::bin2ip(const qint64& iIPAddress)
{
	QString	szIP	= QString("%1.%2.%3.%4").arg((iIPAddress >> 24) & 0xFF).arg((iIPAddress >> 16) & 0xFF).arg((iIPAddress >> 8) & 0xFF).arg(iIPAddress& 0xFF);
	return(szIP);
}
