#include <QtCore>
#include "VjText.h"
#include "NcConstants.h"

//VjText::VjText()
//{
//}

QString VjText::planetName( int planet )
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
  }

  return name;
}
