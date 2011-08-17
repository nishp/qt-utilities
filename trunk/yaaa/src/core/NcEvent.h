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

  NcLocation* location();
  QDateTime dateTime();
  QDate date();
  QTime time();

private:
  NcPimplEvent* d;
};

#endif // _NCEVENT_H_
