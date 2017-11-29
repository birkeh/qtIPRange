#include "clocation.h"


cLocation::cLocation(const qint32 &iLocationID) :
	m_iLocationID(iLocationID),
	m_szName(""),
	m_szLocation(""),
	m_szAlternateLocation(""),
	m_szAddress(""),
	m_iCountryID(-1),
	m_iFederalStateID(-1),
	m_iCityID(-1)
{
}

qint32 cLocation::locationID()
{
	return(m_iLocationID);
}

void cLocation::setName(const QString& szName)
{
	m_szName	= szName;
}

QString cLocation::name()
{
	return(m_szName);
}

void cLocation::setLocation(const QString& szLocation)
{
	m_szLocation	= szLocation;
}

QString cLocation::location()
{
	return(m_szLocation);
}

void cLocation::setAlternateLocation(const QString& szAlternateLocation)
{
	m_szAlternateLocation	= szAlternateLocation;
}

QString cLocation::alternateLocation()
{
	return(m_szAlternateLocation);
}

void cLocation::setAddress(const QString& szAddress)
{
	m_szAddress	= szAddress;
}

QString cLocation::address()
{
	return(m_szAddress);
}

void cLocation::setCountryID(const qint32& iCountryID)
{
	m_iCountryID	= iCountryID;
}

qint32 cLocation::countryID()
{
	return(m_iCountryID);
}

void cLocation::setFederalStateID(const qint32& iFederalStateID)
{
	m_iFederalStateID	= iFederalStateID;
}

qint32 cLocation::federalStateID()
{
	return(m_iFederalStateID);
}

void cLocation::setCityID(const qint32& iCityID)
{
	m_iCityID	= iCityID;
}

qint32 cLocation::cityID()
{
	return(m_iCityID);
}

cLocation* cLocationList::add(const qint32& iLocationID)
{
	cLocation*	lpNew	= 0;

	for(int x = 0;x < count();x++)
	{
		if(at(x)->locationID() == iLocationID)
			return(at(x));
	}

	lpNew	= new cLocation(iLocationID);
	append(lpNew);
	return(lpNew);
}

cLocation* cLocationList::find(const qint32& iID)
{
	for(int x = 0;x < count();x++)
	{
		if(at(x)->locationID() == iID)
			return(at(x));
	}

	return(0);
}
