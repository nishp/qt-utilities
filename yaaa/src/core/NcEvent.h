#ifndef _NCEVENT_H_
#define _NCEVENT_H_

#include <QDateTime>

class NcLocation;
class NcPimplEvent;
class NcEvent
{
public:
  NcEvent();
  ~NcEvent();

  void update();

  void setDateTime( QDateTime dateTime );
  NcLocation* location();
  QDateTime dateTime();
  QDate date();
  QTime time();

  int houseForPlanet( int planet );
  int rashi( int planet );

private:
  NcPimplEvent* d;
};

#endif // _NCEVENT_H_
