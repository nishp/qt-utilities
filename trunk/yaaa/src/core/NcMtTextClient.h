#ifndef NCMTTEXTCLIENT_H
#define NCMTTEXTCLIENT_H

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/dir.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/tokenzr.h>

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
class NcMtTextClient
{
public:

  NcMtTextClient();

  ~NcMtTextClient();

  void run();

protected:
  Horoscope *h, *h2;
  ChartProperties *chartprops;
  DataSet dataset;
  Writer *writer;
  TextHelper *texthelper;
  int writecount;


  void showEclipses();

  void showHora();

  void showEphemeris( const int &type, const int dasa = 0  );

  void showPlanets();

  void showYogas();

  void showTajaka( const int &tajakayear );

  void showPrintoutSummary( int type );

  void showTextAnalysis( const int &mode, const int varga = 0, const int dasa = 0 );

  void showTransits( const int &transitmode, const double &jd );

  void showUranian( const int &type, const int year = 0 );

  void showPartnerAnalysis( const int &type );


  int getIntFromString( wxString s );

  void setDate( wxString s );

  void setLocation( wxString s );

};

#endif // NCMTTEXTCLIENT_H
