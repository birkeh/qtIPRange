#include "ciprange.h"

#include <QFile>
#include <QDomDocument>
#include <algorithm>


bool rangeSort(cIPRange* lpRange1, cIPRange* lpRange2)
{
	if(lpRange1->IPAddressBase().IPAddressBin() < lpRange2->IPAddressBase().IPAddressBin())
		return(true);
	else if(lpRange1->IPAddressBase().IPAddressBin() == lpRange2->IPAddressBase().IPAddressBin())
	{
		if(lpRange1->prefixBase() > lpRange2->prefixBase())
			return(true);
		else if(lpRange1->prefixBase() == lpRange2->prefixBase())
		{
			if(lpRange1->IPAddress().IPAddressBin() < lpRange2->IPAddress().IPAddressBin())
				return(true);
			else if(lpRange1->IPAddress().IPAddressBin() == lpRange2->IPAddress().IPAddressBin())
			{
				if(lpRange1->prefix() > lpRange2->prefix())
					return(true);
			}
		}
	}
	return(false);
}

cIPRange::cIPRange()
{
	setIPRange("0.0.0.0/24");
}

cIPRange::cIPRange(const QString& szIPRange)
{
	setIPRange(szIPRange);
}

bool cIPRange::setIPRange(const QString& szIPRange)
{
	if(!szIPRange.contains("/"))
		return(false);

	QString	szIPAddress;
	QString	szPrefix;

	szIPAddress	= szIPRange.left(szIPRange.indexOf("/"));
	szPrefix	= szIPRange.mid(szIPAddress.length()+1);

	if(!cIPAddress::isValid(szIPAddress))
		return(false);

	if(szPrefix.toInt() < 1 || szPrefix.toInt() > 32)
		return(false);

	m_IPAddress.setIPAddress(cIPAddress::ip2bin(szIPAddress) & ~((1 << (32 - m_iPrefix))-1));
	m_iPrefix	= szPrefix.toInt();

	return(true);
}

QString cIPRange::firstIPAddress()
{
	return(cIPAddress::bin2ip(firstIPAddressBin()));
}

QString cIPRange::lastIPAddress()
{
	return(cIPAddress::bin2ip(lastIPAddressBin()));
}

qint64 cIPRange::firstIPAddressBin()
{
	return((m_IPAddress.IPAddressBin() & netmaskBin())+1);
}

qint64 cIPRange::lastIPAddressBin()
{
	return((m_IPAddress.IPAddressBin() | ~netmaskBin())-1);
}

QString cIPRange::broadcastIPAddress()
{
	return(cIPAddress::bin2ip(m_IPAddress.IPAddressBin() | ~netmaskBin()));
}

qint64 cIPRange::broadcastIPAddressBIN()
{
	return(m_IPAddress.IPAddressBin() | ~netmaskBin());
}

qint32 cIPRange::netmaskBin()
{
	return(~((1 << (32 - m_iPrefix))-1));
}

QString cIPRange::netmask()
{
	return(cIPAddress::bin2ip(netmaskBin()));
}

cIPAddress cIPRange::IPAddress()
{
	return(m_IPAddress);
}

qint16 cIPRange::prefix()
{
	return(m_iPrefix);
}

QString cIPRange::range()
{
	return(QString("%1/%2").arg(m_IPAddress.IPAddress()).arg(m_iPrefix));
}

QString cIPRange::name()
{
	return(m_szName);
}

cIPAddress cIPRange::IPAddressBase()
{
	return(m_IPAddressBase);
}

qint16 cIPRange::prefixBase()
{
	return(m_iPrefixBase);
}

QString cIPRange::rangeBase()
{
	return(QString("%1/%2").arg(m_IPAddressBase.IPAddress()).arg(m_iPrefixBase));
}

void cIPRange::setName(QString& szName)
{
	m_szName	= szName;
}

void cIPRange::setBaseRange(QString &szIPRange)
{
	if(!szIPRange.contains("/"))
		return;

	QString	szIPAddress;
	QString	szPrefix;

	szIPAddress	= szIPRange.left(szIPRange.indexOf("/"));
	szPrefix	= szIPRange.mid(szIPAddress.length()+1);

	if(!cIPAddress::isValid(szIPAddress))
		return;

	if(szPrefix.toInt() < 1 || szPrefix.toInt() > 32)
		return;

	m_IPAddressBase.setIPAddress(szIPAddress);
	m_iPrefixBase	= szPrefix.toInt();
}

void cIPRangeList::clean()
{
	clear();
}

cIPRange* cIPRangeList::add(const QString& szIPRange)
{
	cIPRange*	lpNew	= new cIPRange(szIPRange);

	for(int x = 0;x < count();x++)
	{
		if(lpNew->firstIPAddressBin() == at(x)->firstIPAddressBin() &&
				lpNew->lastIPAddressBin() == at(x)->lastIPAddressBin())
		{
			delete lpNew;
			return(at(x));
		}
	}
	append(lpNew);

	return(lpNew);
}

bool cIPRangeList::load(const QString& szFilename)
{
	clean();

	QFile				file(szFilename);
	if(!file.open(QFile::ReadOnly | QFile::Text))
		return(false);

	QDomDocument		doc;
	QString				errorStr;
	int					errorLine;
	int					errorColumn;
	if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
		return(false);

	file.close();

	QDomElement			root	= doc.documentElement();
	if(root.tagName().compare("IPRanges", Qt::CaseInsensitive))
		return(false);

	QDomNode			child				= root.firstChild();

	while(!child.isNull())
	{
		if(!child.toElement().tagName().compare("IPRange", Qt::CaseInsensitive))
		{
			QDomElement	element	= child.toElement();
			QString	szIPRange	= element.attribute("IPRange");
			QString	szName;
			QString	szLocation;
			QString	szAddress;
			QString	szCity;
			QString	szRange;

			QDomNode	child1	= element.firstChild();

			while(!child1.isNull())
			{
				if(!child1.toElement().tagName().compare("Name", Qt::CaseInsensitive))
					szName		= child1.toElement().text();
//				else if(!child1.toElement().tagName().compare("Location", Qt::CaseInsensitive))
//					szLocation	= child1.toElement().text();
//				else if(!child1.toElement().tagName().compare("Address", Qt::CaseInsensitive))
//					szAddress	= child1.toElement().text();
//				else if(!child1.toElement().tagName().compare("City", Qt::CaseInsensitive))
//					szCity		= child1.toElement().text();
				else if(!child1.toElement().tagName().compare("Range", Qt::CaseInsensitive))
					szRange		= child1.toElement().text();

				child1	= child1.nextSibling();
			}

			cIPRange*	lpIPRange	= add(szRange);
			lpIPRange->setName(szName);
//			lpIPRange->setLocation(szLocation);
//			lpIPRange->setAddress(szAddress);
//			lpIPRange->setCity(szCity);
			lpIPRange->setBaseRange(szIPRange);
		}

		child	= child.nextSibling();
	}

	std::sort(begin(), end(), rangeSort);

	return(true);
}

bool cIPRangeList::fillList(QStandardItemModel* lpModel)
{
	QStandardItem*	lpRoot	= 0;
	QString			szOldRange("");

	for(int z = 0;z < count();z++)
	{
		cIPRange*	lpRange		= at(z);

		if(lpRange->rangeBase() != szOldRange)
		{
			szOldRange	= lpRange->rangeBase();
			lpRoot			= new QStandardItem(szOldRange);
//			QVariant	v	= qVariantFromValue(lpVideos->set());
//			lpRoot->setData(v, Qt::UserRole);
			lpModel->appendRow(lpRoot);
		}

		QList<QStandardItem*>	lpItems;
		lpItems.append(new QStandardItem(lpRange->range()));
		lpItems.append(new QStandardItem(lpRange->name()));
//		lpItems.append(new QStandardItem(lpRange->location()));
//		lpItems.append(new QStandardItem(lpRange->address()));
//		lpItems.append(new QStandardItem(lpRange->city()));

		QVariant	v	= qVariantFromValue(lpRange);
		lpItems.at(0)->setData(v, Qt::UserRole);
		lpRoot->appendRow(lpItems);
	}

	return(true);
}
