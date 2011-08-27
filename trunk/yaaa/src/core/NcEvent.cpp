#include "NcLocation.h"
#include "NcEvent.h"
#include "NcMaitreyaAdapter.h"

class NcPimplEvent
{
public:
  NcLocation location;
  QDateTime dateTime;
  NcMaitreyaAdapter adpt;
};

NcEvent::NcEvent()
  :d( new NcPimplEvent )
{
}

NcEvent::~NcEvent()
{
  delete d;
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

void NcEvent::setDateTime(QDateTime dateTime)
{
  d->dateTime = dateTime;
}

void NcEvent::update()
{
  d->adpt.setEvent(this);
}
