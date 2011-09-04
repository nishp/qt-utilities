#ifndef _NCCONSTANTS_H_
#define _NCCONSTANTS_H_

namespace Nc
{

enum enPlanets
{
  Sun = 0,
  Moon,
  Mars,
  Mercury,
  Jupiter,
  Venus,
  Saturn,
  Rahu,
  Ketu,
  Ascendant,
  PlanetsSize
};

enum enHouses
{
  House_01 = 0,
  House_02,
  House_03,
  House_04,
  House_05,
  House_06,
  House_07,
  House_08,
  House_09,
  House_10,
  House_11,
  House_12,
  HousesSize
};

enum enSigns
{
  Aries,
  Taurus,
  Gemini,
  Cancer,
  Leo,
  Virgo,
  Libra,
  Scorpio,
  Sagittarius,
  Capricorn,
  Aquarius,
  Pisces,
  SignSize
};

}//end Namespace Nc

class NcConstants
{
public:
  static int nextRashi( int rashi )
  {
    ++rashi;
    if ( rashi > Nc::Pisces )
      return Nc::Aries;
    else
      return rashi;
  }
};


#endif // _NCCONSTANTS_H_
