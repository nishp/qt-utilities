#ifndef _NCPERSON_H_
#define _NCPERSON_H_

#include <QString>

class NcEvent;
class NcPimplPerson;
class NcPerson
{
public:
  NcPerson();
  ~NcPerson();

  NcEvent* birthEvent();
  QString fullName();
  QString firstName();
  QString midName();
  QString lastName();

  void setFirstName( const QString &firstName );
  void setMidName( const QString &midName );
  void setLastName( const QString &lastName );

private:
  NcPimplPerson* d;
};

#endif // _NCPERSON_H_
