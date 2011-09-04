#include <QtGui>
#include "NcTestGui.h"
#include "NcGfx2D.h"
#include "NcPerson.h"
#include "NcEvent.h"
#include "NcText.h"

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
  QTime t;
  t.start();

  QPainter p(this);
  NcChartData chartData;
  NcText text;
  NcEvent *event = person->birthEvent();
  int house;

  qDebug() << "chart";
  for ( int i = Nc::Sun; i < Nc::PlanetsSize; ++i )
  {
    house = event->houseForPlanet( i );
    chartData.planetsInHouse[house] += " " + text.planetName(i);
    qDebug() << i << text.planetName(i) << "=" << house;
  }
  chartData.asc = event->rashi( Nc::Ascendant );

  gfx->drawChart( &chartData, &p);

  qDebug() << "elapsed = " << t.elapsed();
}
