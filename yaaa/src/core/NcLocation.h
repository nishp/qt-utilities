#ifndef _NCLOCATION_H_
#define _NCLOCATION_H_

#include <QString>

class NcPimplLocation;
class NcLocation
{
public:
  NcLocation();
  ~NcLocation();

  double latitude();
  void setLatitude( double newLatitude );

  double longitude();
  void setLongitude( double newLongitude );

  float tz();
  void setTz( float newTz );

  float dst();
  void setDst( float newDst );

  QString country();
  void setCountry( QString newCountry );

  QString state();
  void setState( QString newState );

  QString district();
  void setDistrict( QString newDistrict );

  QString city();
  void setCity( QString newCity );

private:
  NcPimplLocation* d;
};

#endif // _NCLOCATION_H_
