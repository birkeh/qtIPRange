#include "clocation.h"


cLocation::cLocation(const qint32 &iLocationID) :
	m_iLocationID(iLocationID),
	m_szName(""),
	m_szLocation(""),
	m_szAlternateLocation(""),
	m_szAddress(""),
	m_szCountry(""),
	m_szFederalState(""),
	m_szCity(""),
	m_iPostalCode(-1)
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

void cLocation::setCountry(const QString& szCountry)
{
	m_szCountry	= szCountry;
}

QString cLocation::country()
{
	return(m_szCountry);
}

void cLocation::setFederalState(const QString& szFederalState)
{
	m_szFederalState	= szFederalState;
}

QString cLocation::federalState()
{
	return(m_szFederalState);
}

void cLocation::setCity(const QString& szCity)
{
	m_szCity	= szCity;
}

QString cLocation::city()
{
	return(m_szCity);
}

void cLocation::setPostalCode(const qint16& iPostalCode)
{
	m_iPostalCode	= iPostalCode;
}

qint16 cLocation::postalCode()
{
	return(m_iPostalCode);
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
