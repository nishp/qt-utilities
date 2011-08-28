#ifndef _NCGFX2D_H_
#define _NCGFX2D_H_

#include <QString>
#include "NcConstants.h"

class QPainter;

struct VjChartData
{
  QString planetsInHouse[Vj::HousesSize];
};

class NcGfx2D
{
public:
  NcGfx2D();
  ~NcGfx2D();

  void drawChart(VjChartData *chartData,QPainter* pntr);

private:

};

/*
p0----------p1----------p2
|
|     p3          p4
|
|
p5           p6          p7

      p8           p9   	|
|
|
p10----------p11---------p12

*/

#endif // _NCGFX2D_H_
