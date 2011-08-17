#include "NcLocation.h"
#include "NcEvent.h"

class NcPimplEvent
{
public:
  NcLocation location;
  QDateTime dateTime;
};

NcEvent::NcEvent()
{
}

NcEvent::~NcEvent()
{
}

NcLocation * NcEvent::location()
{
  return &d->location;
}

QDateTime NcEvent::dateTime()
{
  return d->dateTime;
}

QDate NcEvent::date()
{
  return d->dateTime.date();
}

QTime NcEvent::time()
{
  return d->dateTime.time();
}
