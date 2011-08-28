#include <QtGui>
#include "NcTestGui.h"
#include "NcGfx2D.h"
#include "NcPerson.h"
#include "NcEvent.h"
#include "VjText.h"

TestDialog::TestDialog( NcPerson *person, QWidget* parent) : QDialog(parent)
{
  this->person = person;
  gfx = new NcGfx2D;
}

TestDialog::~TestDialog()
{
  delete gfx;
}

void TestDialog::paintEvent(QPaintEvent *)
{
  QPainter p(this);
  VjChartData chartData;
  VjText text;
  NcEvent *event = person->birthEvent();
  int house;

  for ( int i = Vj::PlanetsStart; i < Vj::PlanetsSize; ++i )
  {
    house = event->houseForPlanet( i );
    chartData.planetsInHouse[house] += " " + text.planetName(i);
  }

  gfx->drawChart( &chartData, &p);
}
