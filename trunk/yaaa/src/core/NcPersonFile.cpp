#include <QtCore>
#include "NcPersonFile.h"
#include "NcPerson.h"
#include "NcEvent.h"
#include "NcLocation.h"

#define NC_DATE_FORMAT "yyyy.MM.dd"
#define NC_TIME_FORMAT "hh.mm.ss"

#define NC_TAG_FIRSTNAME "FirstName"
#define NC_TAG_MIDNAME   "MidName"
#define NC_TAG_LASTNAME  "LastName"
#define NC_TAG_DATE      "Date"
#define NC_TAG_TIME      "Time"
#define NC_TAG_COUNTRY   "Country"
#define NC_TAG_STATE     "State"
#define NC_TAG_DISTRICT  "District"
#define NC_TAG_CITY      "City"
#define NC_TAG_LONGITUDE "Longitude"
#define NC_TAG_LATITIDE  "Latitude"
#define NC_TAG_TIMEZONE  "TimeZone"
#define NC_TAG_DST       "Dst"


class NcPimplPersonFile
{
public:
  int lastError;
  QString strLastError;

  void setError( int errorNo, QString errorDescription );
  void resetError();
};

void NcPimplPersonFile::setError(int errorNo, QString errorDescription)
{
  this->lastError = errorNo;
  this->strLastError = errorDescription;
}

void NcPimplPersonFile::resetError()
{
  this->lastError = NcPersonFile::ERROR_OK;
  this->strLastError = "No Error";
}

NcPersonFile::NcPersonFile()
  : d( new NcPimplPersonFile )
{
}

NcPersonFile::~NcPersonFile()
{
  delete d;
}

QString NcPersonFile::lastErrorString()
{
  return d->strLastError;
}

bool NcPersonFile::savePerson(const NcPerson *person, QString filePath)
{
  d->resetError();

  QFile file(filePath);

  if ( !file.open(QFile::WriteOnly|QFile::Text) )
  {
    QString des = "Could not open file ";
    des += file.fileName() + "for writing.";
    d->setError( ERROR_FILE_ACCESS_DENIED, des );
    return false;
  }

  file.close();


  NcEvent* event = person->birthEvent();
  NcLocation* loc = event->location();

  QSettings settings( filePath, QSettings::IniFormat );

  settings.setValue( NC_TAG_FIRSTNAME, person->firstName() );
  settings.setValue( NC_TAG_MIDNAME, person->midName() );
  settings.setValue( NC_TAG_LASTNAME, person->lastName() );
  settings.setValue( NC_TAG_DATE, event->date().toString( NC_DATE_FORMAT ) );
  settings.setValue( NC_TAG_TIME, event->time().toString( NC_TIME_FORMAT ) );
  settings.setValue( NC_TAG_COUNTRY, loc->country() );
  settings.setValue( NC_TAG_STATE, loc->state() );
  settings.setValue( NC_TAG_DISTRICT, loc->district() );
  settings.setValue( NC_TAG_CITY,loc->city() );
  settings.setValue( NC_TAG_LONGITUDE, loc->longitude() );
  settings.setValue( NC_TAG_LATITIDE, loc->latitude() );
  settings.setValue( NC_TAG_TIMEZONE, loc->tz() );
  settings.setValue( NC_TAG_DST, loc->dst() );

  return true;

}

bool NcPersonFile::loadPerson( QString filePath, NcPerson *person)
{
  //TODO, make a default value set

  d->resetError();

  QSettings settings( filePath, QSettings::IniFormat );

  person->setFirstName( settings.value( NC_TAG_FIRSTNAME, "Error" ).toString() );
  person->setMidName( settings.value( NC_TAG_MIDNAME, "Error" ).toString() );
  person->setLastName( settings.value( NC_TAG_LASTNAME, "Error" ).toString() );

  NcEvent* event = person->birthEvent();
  NcLocation* loc = event->location();

  QString strDate = QDate::currentDate().toString( NC_DATE_FORMAT );
  QString strTime = QTime::currentTime().toString( NC_TIME_FORMAT );

  strDate = settings.value( NC_TAG_DATE, strDate ).toString().simplified();
  strTime = settings.value( NC_TAG_TIME, strTime ).toString().simplified();

  QDate date = QDate::fromString( strDate, NC_DATE_FORMAT );
  QTime time = QTime::fromString( strDate, NC_TIME_FORMAT );

  event->setDateTime( QDateTime(date,time) );

  loc->setCountry( settings.value( NC_TAG_COUNTRY, "Error" ).toString() );
  loc->setState( settings.value( NC_TAG_STATE, "Error").toString() );
  loc->setDistrict( settings.value( NC_TAG_DISTRICT, "Error" ).toString() );
  loc->setCity( settings.value( NC_TAG_CITY, "Error" ).toString() );
  loc->setLongitude( settings.value( NC_TAG_LONGITUDE, 0 ).toDouble() );
  loc->setLatitude( settings.value( NC_TAG_LATITIDE, 0 ).toDouble() );
  loc->setTz( settings.value( NC_TAG_TIMEZONE, 0 ).toDouble() );
  loc->setDst( settings.value( NC_TAG_DST, 0 ).toDouble() );

  return true;
}


