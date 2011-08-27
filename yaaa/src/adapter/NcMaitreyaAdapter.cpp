#include "Aspect.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Eclipse.h"
#include "Ephemeris.h"
#include "Hora.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Partner.h"
#include "PrintoutHelper.h"
#include "FileConfig.h"
#include "TextHelper.h"
#include "Transit.h"
#include "Uranian.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"
#include "Yoga.h"

#include <QtCore>
#include "NcEvent.h"
#include "NcLocation.h"
#include "NcMaitreyaAdapter.h"

class NcPimplMaitreyaAdapter
{
public:
  NcEvent *event;
  Horoscope* h;
  ChartProperties* chartprops;
  DataSet dataset;
};

NcMaitreyaAdapter::NcMaitreyaAdapter()
  :d( new NcPimplMaitreyaAdapter )
{

}


NcMaitreyaAdapter::~NcMaitreyaAdapter()
{
  delete d;
}
extern Config *config;
void NcMaitreyaAdapter::setEvent(NcEvent *event)
{
  //int c = config->activePageAshtakaVarga;
  d->event = event;

  NcLocation* loc = d->event->location();
  QDate date = d->event->date();
  QTime time = d->event->time();

  double fTime = 0;
  fTime += time.hour() * 60 * 60;
  fTime += time.minute() * 60;
  fTime += time.second();

  wxString basedir = wxT("/home/nish/svn-google/qt-utilities/yaaa/src/maitreya");

  FileConfig::get()->init( basedir );

  wxString name( loc->city().toAscii().constData(), wxConvUTF8 );
  d->dataset.setLocation( name, loc->longitude(), loc->latitude(), loc->tz(), loc->dst() );
  d->dataset.setDate( date.day(), date.month(), date.year(), fTime );

  //Calculator *calculator = CalculatorFactory().getCalculator();

  d->chartprops = new ChartProperties;
  d->h = new Horoscope;

  d->chartprops->setVedic();

  d->h->setDate( d->dataset.getJD());
  d->h->setLocation( *(d->dataset.getLocation()) );
  d->h->update();


  vector<int> obs = d->chartprops->getPlanetList();

  int i;
  for( unsigned int p = 0; p < obs.size(); p++ )
  {
    i = obs[p];
    qDebug() << d->h->isRetrograde( i );
    qDebug() << d->h->getHousePos( i, true );
  }

}

int NcMaitreyaAdapter::houseForPlanet(int planet)
{

}
