#ifndef CLOCATION_H
#define CLOCATION_H


#include <QMetaType>
#include <QString>
#include <QList>


class cLocation
{
public:
	cLocation(const qint32& iLocationID);

	qint32		locationID();

	void		setName(const QString& szName);
	QString		name();

	void		setLocation(const QString& szLocation);
	QString		location();

	void		setAlternateLocation(const QString& szAlternateLocation);
	QString		alternateLocation();

	void		setAddress(const QString& szAddress);
	QString		address();

	void		setCountry(const QString& szCountry);
	QString		country();

	void		setFederalState(const QString& szFederalState);
	QString		federalState();

	void		setCity(const QString& szCity);
	QString		city();

	void		setPostalCode(const qint16& iPostalCode);
	qint16		postalCode();

private:
	qint32		m_iLocationID;
	QString		m_szName;
	QString		m_szLocation;
	QString		m_szAlternateLocation;
	QString		m_szAddress;
	QString		m_szCountry;
	QString		m_szFederalState;
	QString		m_szCity;
	qint16		m_iPostalCode;
};

Q_DECLARE_METATYPE(cLocation*)

class cLocationList : public QList<cLocation*>
{
public:
	cLocation*		add(const qint32& iLocationID);
	cLocation*		find(const qint32& iID);
};

Q_DECLARE_METATYPE(cLocationList*)

#endif // CLOCATION_H
