#ifndef _NCTEXT_H_
#define _NCTEXT_H_

#include <QString>
class NcText
{
public:
  //VjText();
  QString planetName( int planet );
  QString planetNameShort( int planet );
  QString planetNameLong( int planet );

  QString rashiNumber( int rashi );
  QString rashiNameLong( int rashi );
};

#endif // _NCTEXT_H_
