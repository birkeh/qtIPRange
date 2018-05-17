#ifndef CLOCATION_H
#define CLOCATION_H


#include <QMetaType>
#include <QString>
#include <QList>


/*!
 \brief The cLocation class provides values corresponding to a location.

 \class cLocation clocation.h "clocation.h"
*/
class cLocation
{
public:
	/*!
	 \brief Creates a cLocation and initializes with the given <i>iLocationID</i>.

	 The location values may be set seperately.

	 \fn cLocation
	*/
	cLocation(const qint32& iLocationID);

	/*!
	 \brief Returns the ID of the location.

	 \fn locationID
	 \return ID of the location.
	*/
	qint32		locationID();

	/*!
	 \brief Sets the name of the location.

	 \fn setName
	 \param szName Name of the location.
	*/
	void		setName(const QString& szName);
	/*!
	 \brief Returns the name of the location.

	 \fn name
	 \return Name of the location.
	*/
	QString		name();

	/*!
	 \brief Sets the location name of the location.

	 \fn setLocation
	 \param szLocation Location name of the location.
	*/
	void		setLocation(const QString& szLocation);
	/*!
	 \brief Returns the location name of the location.

	 \fn location
	 \return Location name of the location.
	*/
	QString		location();

	/*!
	 \brief Sets the alternate location name of the location. This value is used to override possible unreadable location names.

	 \fn setAlternateLocation
	 \param szAlternateLocation Alternate location name of the location.
	*/
	void		setAlternateLocation(const QString& szAlternateLocation);
	/*!
	 \brief Returns the alternate location name of the location.

	 \fn alternateLocation
	 \return Alternate location name of the location.
	*/
	QString		alternateLocation();

	/*!
	 \brief Sets the address of the location.

	 \fn setAddress
	 \param szAddress Address of the location.
	*/
	void		setAddress(const QString& szAddress);
	/*!
	 \brief Returns the address of this location.

	 \fn address
	 \return Address of this location.
	*/
	QString		address();

	/*!
	 \brief Sets the name of the country of this location.

	 \fn setCountry
	 \param szCountry Name of the country.
	*/
	void		setCountry(const QString& szCountry);
	/*!
	 \brief Returns the name of the country of this location.

	 \fn country
	 \return Name of the country.
	*/
	QString		country();

	/*!
	 \brief Sets the name of the federal state of this location.

	 \fn setFederalState
	 \param szFederalState Name of the federal state.
	*/
	void		setFederalState(const QString& szFederalState);
	/*!
	 \brief Returns the name of the federal state of this location.

	 \fn federalState
	 \return Name of the federal state.
	*/
	QString		federalState();

	/*!
	 \brief Sets the name of the city of this location.

	 \fn setCity
	 \param szCity Name of the city.
	*/
	void		setCity(const QString& szCity);
	/*!
	 \brief Returns the name of the city of this location.

	 \fn city
	 \return Name of the city.
	*/
	QString		city();

	/*!
	 \brief Sets the postal code of this location.

	 \fn setPostalCode
	 \param iPostalCode Postal code of this location.
	*/
	void		setPostalCode(const qint16& iPostalCode);
	/*!
	 \brief Returns the postal code of this location.

	 \fn postalCode
	 \return Postal code of this location.
	*/
	qint16		postalCode();

private:
	qint32		m_iLocationID;			/*!< This property holds the ID of this location.<br><b>Access functions:</b><br><table><tr><td>qint32</td><td><b>locationID</b>()</td></tr><tr><td></td><td><b>cLocation</b>(const qint32& iLocationID)</td></tr></table> */
	QString		m_szName;				/*!< This property holds the name of this location.<br><b>Access functions:</b><br><table><tr><td>QString</td><td><b>name</b>()</td></tr><tr><td>void</td><td><b>setName</b>(const QString& szName)</td></tr></table> */
	QString		m_szLocation;			/*!< This property holds the location name of this location.<br><b>Access functions:</b><br><table><tr><td>QString</td><td><b>locationName</b>()</td></tr><tr><td>void</td><td><b>setLocationName</b>(const QString& szLocationName)</td></tr></table> */
	QString		m_szAlternateLocation;	/*!< This property holds the alternate location name of this location.<br><b>Access functions:</b><br><table><tr><td>QString</td><td><b>alternateName</b>()</td></tr><tr><td>void</td><td><b>setAlternateName</b>(const QString& szAlternateName)</td></tr></table> */
	QString		m_szAddress;			/*!< This property holds the address of this location.<br><b>Access functions:</b><br><table><tr><td>QString</td><td><b>address</b>()</td></tr><tr><td>void</td><td><b>setAddress</b>(const QString& szAddress)</td></tr></table> */
	QString		m_szCountry;			/*!< This property holds the country of this location.<br><b>Access functions:</b><br><table><tr><td>QString</td><td><b>country</b>()</td></tr><tr><td>void</td><td><b>setCountry</b>(const QString& szCountry)</td></tr></table> */
	QString		m_szFederalState;		/*!< This property holds the federal state of this location.<br><b>Access functions:</b><br><table><tr><td>QString</td><td><b>federalState</b>()</td></tr><tr><td>void</td><td><b>setFederalState</b>(const QString& szFederalState)</td></tr></table> */
	QString		m_szCity;				/*!< This property holds the city of this location.<br><b>Access functions:</b><br><table><tr><td>QString</td><td><b>city</b>()</td></tr><tr><td>void</td><td><b>setCity</b>(const QString& szCity)</td></tr></table> */
	qint16		m_iPostalCode;			/*!< This property holds the postal code of this location.<br><b>Access functions:</b><br><table><tr><td>qint16</td><td><b>postalCode</b>()</td></tr><tr><td>void</td><td><b>setPostalCode</b>(const qint16& iPostalCode)</td></tr></table> */
};

Q_DECLARE_METATYPE(cLocation*)

/*!
 \brief The cLocationList class provides a list of locations.

 \class cLocationList clocation.h "clocation.h"
*/
class cLocationList : public QList<cLocation*>
{
public:
	/*!
	 \brief Adds a new location to the list.

	 If the location already exists in the list the existing location is returned.

	 \fn add
	 \param iLocationID ID of the location to be added.
	 \return pointer to the newly created or already existing location.
	*/
	cLocation*		add(const qint32& iLocationID);
	/*!
	 \brief Searches for a given <i>location</i> within the list.

	 Returns the found location or NULL if nothing is found.

	 \fn find
	 \param iID ID of the location to be found.
	 \return pointer to the found location or NULL if nothing is found.
	*/
	cLocation*		find(const qint32& iID);
};

Q_DECLARE_METATYPE(cLocationList*)

#endif // CLOCATION_H
