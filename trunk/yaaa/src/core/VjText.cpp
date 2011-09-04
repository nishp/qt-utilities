#include <QtCore>
#include "VjText.h"
#include "NcConstants.h"

//VjText::VjText()
//{
//}

QString VjText::planetName( int planet )
{
  return planetNameShort( planet );
}

QString VjText::planetNameShort( int planet )
{
  QString name = QObject::tr("Error");
  switch ( planet )
  {
    case Vj::Sun:
      name = QObject::tr("Su"); break;
    case Vj::Moon:
      name = QObject::tr("Mo"); break;
    case Vj::Mars:
      name = QObject::tr("Ma"); break;
    case Vj::Mercury:
      name = QObject::tr("Me"); break;
    case Vj::Jupiter:
      name = QObject::tr("Ju"); break;
    case Vj::Venus:
      name = QObject::tr("Ve"); break;
    case Vj::Saturn:
      name = QObject::tr("Sa"); break;
    case Vj::Rahu:
      name = QObject::tr("Ra"); break;
    case Vj::Ketu:
      name = QObject::tr("Ke"); break;
    case Vj::Ascendant:
      name = QObject::tr("Asc"); break;
  }

  return name;
}

QString VjText::planetNameLong( int planet )
{
  QString name = QObject::tr("Error");
  switch ( planet )
  {
    case Vj::Sun:
      name = QObject::tr("Sun"); break;
    case Vj::Moon:
      name = QObject::tr("Moon"); break;
    case Vj::Mars:
      name = QObject::tr("Mars"); break;
    case Vj::Mercury:
      name = QObject::tr("Mercury"); break;
    case Vj::Jupiter:
      name = QObject::tr("Jupiter"); break;
    case Vj::Venus:
      name = QObject::tr("Venus"); break;
    case Vj::Saturn:
      name = QObject::tr("Saturn"); break;
    case Vj::Rahu:
      name = QObject::tr("Rahu"); break;
    case Vj::Ketu:
      name = QObject::tr("Ketu"); break;
    case Vj::Ascendant:
      name = QObject::tr("Asc"); break;
  }

  return name;
}
