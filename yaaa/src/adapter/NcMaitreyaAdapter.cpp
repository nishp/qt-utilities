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
#include "NcConstants.h"
#include "NcMtTextClient.h"

class NcPimplMaitreyaAdapter
{
public:
  NcEvent *event;
  Horoscope* h;
  VargaHoroscope* vh;
  ChartProperties* chartprops;
  DataSet dataset;
};

int getMtPlanet( int& vjPlanet )
{
  switch ( vjPlanet )
  {
    default: qDebug() << "Error in getMtPlanet";
    case Nc::Sun:       return OSUN;
    case Nc::Moon:      return OMOON;
    case Nc::Mars:      return OMARS;
    case Nc::Mercury:   return OMERCURY;
    case Nc::Jupiter:   return OJUPITER;
    case Nc::Venus:     return OVENUS;
    case Nc::Saturn:    return OSATURN;
    case Nc::Rahu:      return OTRUENODE;
    case Nc::Ketu:      return OTRUEDESCNODE;
    case Nc::Ascendant: return OASCENDANT;
  }
}

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

  qDebug() << "dt=" << d->event->dateTime();

  double fTime = 0;
  fTime += time.hour();// * 60 * 60;
  fTime += time.minute() / 60;
  fTime += (time.second() / 60) / 60;

  fTime -= loc->tz() - loc->dst();

  qDebug() << "ftime = " << fTime;

  wxString basedir = wxT("/home/nish/svn-google/qt-utilities/yaaa/src/maitreya");

  FileConfig::get()->init( basedir );

  //NcMtTextClient tc;
  //tc.run();

  wxString name( loc->city().toAscii().constData(), wxConvUTF8 );
  d->dataset.setLocation( name, loc->longitude(), loc->latitude(), loc->tz(), loc->dst() );
  d->dataset.setDate( date.day(), date.month(), date.year(), fTime );

  FileDataSet ds;

  wxString ff = wxT("/home/nish/Desktop/charts/nishant.mtx");
  ds.openFile(ff);
  ds.getLocation();


  //Calculator *calculator = CalculatorFactory().getCalculator();

  //d->chartprops = new ChartProperties;
  d->h = new Horoscope;

  //d->chartprops->setVedic();

  d->h->setDate( d->dataset.getJD());
  d->h->setLocation( *(d->dataset.getLocation()) );

  //d->h->setDate( ds.getJD());
  //d->h->setLocation( *(ds.getLocation()) );

  //d->h->openFile(ff);
  d->h->update();

  wxString dmp;
  d->h->getDataSet()->dump(dmp);

  qDebug() << "Dump1 = " << dmp.ToAscii().data();

  d->vh = new VargaHoroscope(d->h);


//  vector<int> obs = d->chartprops->getVedicPlanetList();

//  int i;
//  for( unsigned int p = 0; p < obs.size(); p++ )
//  {
//    i = obs[p];
//    //qDebug() << d->h->isRetrograde( i );
//    qDebug() << i << "=" << d->h->getHousePos( i, true );
//  }

}

int NcMaitreyaAdapter::houseForPlanet(int planet)
{
  //return d->h->getHousePos( getMtPlanet(planet), true );
  return d->vh->getBhava( getMtPlanet(planet) );
}

int NcMaitreyaAdapter::rashi(int planet)
{
  return d->vh->getRasi(getMtPlanet(planet));
}


