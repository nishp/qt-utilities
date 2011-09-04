#include <QtCore>
#include "NcText.h"
#include "NcConstants.h"

QString NcText::planetName( int planet )
{
  return planetNameShort( planet );
}

QString NcText::planetNameShort( int planet )
{
  QString name = QObject::tr("Error");
  switch ( planet )
  {
    case Nc::Sun:
      name = QObject::tr("Su"); break;
    case Nc::Moon:
      name = QObject::tr("Mo"); break;
    case Nc::Mars:
      name = QObject::tr("Ma"); break;
    case Nc::Mercury:
      name = QObject::tr("Me"); break;
    case Nc::Jupiter:
      name = QObject::tr("Ju"); break;
    case Nc::Venus:
      name = QObject::tr("Ve"); break;
    case Nc::Saturn:
      name = QObject::tr("Sa"); break;
    case Nc::Rahu:
      name = QObject::tr("Ra"); break;
    case Nc::Ketu:
      name = QObject::tr("Ke"); break;
    case Nc::Ascendant:
      name = QObject::tr("Asc"); break;
  }

  return name;
}

QString NcText::planetNameLong( int planet )
{
  QString name = QObject::tr("Error");
  switch ( planet )
  {
    case Nc::Sun:
      name = QObject::tr("Sun"); break;
    case Nc::Moon:
      name = QObject::tr("Moon"); break;
    case Nc::Mars:
      name = QObject::tr("Mars"); break;
    case Nc::Mercury:
      name = QObject::tr("Mercury"); break;
    case Nc::Jupiter:
      name = QObject::tr("Jupiter"); break;
    case Nc::Venus:
      name = QObject::tr("Venus"); break;
    case Nc::Saturn:
      name = QObject::tr("Saturn"); break;
    case Nc::Rahu:
      name = QObject::tr("Rahu"); break;
    case Nc::Ketu:
      name = QObject::tr("Ketu"); break;
    case Nc::Ascendant:
      name = QObject::tr("Asc"); break;
  }

  return name;
}

QString NcText::rashiNumber(int rashi)
{
  QString num = QObject::tr("-1");
  switch ( rashi )
  {
    case Nc::Aries:
      num = QObject::tr("1"); break;
    case Nc::Taurus:
      num = QObject::tr("2"); break;
    case Nc::Gemini:
      num = QObject::tr("3"); break;
    case Nc::Cancer:
      num = QObject::tr("4"); break;
    case Nc::Leo:
      num = QObject::tr("5"); break;
    case Nc::Virgo:
      num = QObject::tr("6"); break;
    case Nc::Libra:
      num = QObject::tr("7"); break;
    case Nc::Scorpio:
      num = QObject::tr("8"); break;
    case Nc::Sagittarius:
      num = QObject::tr("9"); break;
    case Nc::Capricorn:
      num = QObject::tr("10"); break;
    case Nc::Aquarius:
      num = QObject::tr("11"); break;
    case Nc::Pisces:
      num = QObject::tr("12"); break;
  }

  return num;
}

QString NcText::rashiNameLong(int rashi)
{
  QString name = QObject::tr("error");
  switch ( rashi )
  {
    case Nc::Aries:
      name = QObject::tr("Aries"); break;
    case Nc::Taurus:
      name = QObject::tr("Taurus"); break;
    case Nc::Gemini:
      name = QObject::tr("Gemini"); break;
    case Nc::Cancer:
      name = QObject::tr("Cancer"); break;
    case Nc::Leo:
      name = QObject::tr("Leo"); break;
    case Nc::Virgo:
      name = QObject::tr("Virgo"); break;
    case Nc::Libra:
      name = QObject::tr("Libra"); break;
    case Nc::Scorpio:
      name = QObject::tr("Scorpio"); break;
    case Nc::Sagittarius:
      name = QObject::tr("Sagittarius"); break;
    case Nc::Capricorn:
      name = QObject::tr("Capricorn"); break;
    case Nc::Aquarius:
      name = QObject::tr("Aquarius"); break;
    case Nc::Pisces:
      name = QObject::tr("Pisces"); break;
  }

  return name;
}
