#include <QtCore>
#include "NcEvent.h"
#include "NcPerson.h"

class NcPimplPerson
{
public:
  QString firstName;
  QString midName;
  QString lastName;

  NcEvent birthEvent;
};

NcPerson::NcPerson()
  : d( new NcPimplPerson )
{

}

NcPerson::~NcPerson()
{
  delete d;
}

NcEvent * NcPerson::birthEvent()
{
  return &d->birthEvent;
}

QString NcPerson::fullName()
{
  return d->firstName + " " + d->midName + " " + d->lastName;
}

QString NcPerson::firstName()
{
  return d->firstName;
}

QString NcPerson::midName()
{
  return d->midName;
}

QString NcPerson::lastName()
{
  return d->lastName;
}

void NcPerson::setFirstName(const QString &firstName)
{
  d->firstName = firstName;
}

void NcPerson::setMidName(const QString &midName)
{
  d->midName = midName;
}

void NcPerson::setLastName(const QString &lastName)
{
  d->lastName = lastName;
}

