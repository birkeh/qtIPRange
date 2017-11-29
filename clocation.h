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

	void		setCountryID(const qint32& iCountryID);
	qint32		countryID();

	void		setFederalStateID(const qint32& iFederalStateID);
	qint32		federalStateID();

	void		setCityID(const qint32& iCityID);
	qint32		cityID();
private:
	qint32		m_iLocationID;
	QString		m_szName;
	QString		m_szLocation;
	QString		m_szAlternateLocation;
	QString		m_szAddress;
	qint32		m_iCountryID;
	qint32		m_iFederalStateID;
	qint32		m_iCityID;
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
