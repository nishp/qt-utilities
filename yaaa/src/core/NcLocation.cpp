#include <QtCore>
#include "NcLocation.h"

class NcPimplLocation
{
public:
  double latitude;
  double longitude;
  double tz;
  double dst;

  QString country;
  QString state;
  QString district;
  QString city;
};


NcLocation::NcLocation()
  : d( new NcPimplLocation )
{

}

NcLocation::~NcLocation()
{
  delete d;
}

double NcLocation::latitude()
{
  return d->latitude;
}

void NcLocation::setLatitude(double newLatitude)
{
  d->latitude = newLatitude;
}

double NcLocation::longitude()
{
  return d->longitude;
}

void NcLocation::setLongitude(double newLongitude)
{
  d->longitude = newLongitude;
}

double NcLocation::tz()
{
  return d->tz;
}

void NcLocation::setTz(double newTz)
{
  d->tz = newTz;
}

double NcLocation::dst()
{
  return d->dst;
}

void NcLocation::setDst(double newDst)
{
  d->dst = newDst;
}

QString NcLocation::country()
{
  return d->country;
}

void NcLocation::setCountry(QString newCountry)
{
  d->country = newCountry;
}

QString NcLocation::state()
{
  return d->state;
}

void NcLocation::setState(QString newState)
{
  d->state = newState;
}

QString NcLocation::district()
{
  return d->district;
}

void NcLocation::setDistrict(QString newDistrict)
{
  d->district = newDistrict;
}

QString NcLocation::city()
{
  return d->city;
}

void NcLocation::setCity(QString newCity)
{
  d->city = newCity;
}
