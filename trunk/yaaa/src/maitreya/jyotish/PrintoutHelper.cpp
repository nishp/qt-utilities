/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/PrintoutHelper.cpp
 *  Release    6.0
 *  Author     M. Pettau
 *  Copyright  2003-2010 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
****************************************************************************/

#ifdef __GNUG__
	#pragma implementation "PrintoutHelper.h"
#endif

#include "PrintoutHelper.h"

#include <wx/log.h>
#include <wx/string.h>
#include <math.h>

#include "Ashtakavarga.h"
#include "astrobase.h"
#include "constants.h"
#include "Conf.h"
#include "constants.h"
#include "Horoscope.h"
#include "Jaimini.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "Planet.h"
#include "PlanetList.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   PrintoutHelper   ---   Constructor
**
******************************************************/
PrintoutHelper::PrintoutHelper( const Horoscope *horoscope, Writer *ww )
: h ( horoscope )
{
	writer = ww;
}

/*****************************************************
**
**   PrintoutHelper   ---   Destructor
**
******************************************************/
PrintoutHelper::~PrintoutHelper()
{
	delete table;
}


/*****************************************************
**
**   PrintoutHelper   ---   writeBaseData
**
******************************************************/
void PrintoutHelper::writeBaseData()
{
	wxString s;
	int line = 1;
	double thetime;
	Formatter *f = Formatter::get();

	DataSet *ds = h->getDataSet();
	Location *loc = ds->getLocation();
	JDate *date = ds->getDate();
	table->setHeader( 1,  _( "Base Data" ));

	table->setEntry( 0, line,  _( "Name" ));
	table->setEntry( 1, line++, h->getHName() );

	table->setEntry( 0, line,  _( "Date" ));
	s << f->getDateStringFromJD( h->getJD() + ( loc->getTimeZone() + loc->getDST() )/24 );
	table->setEntry( 1, line++, s );

	table->setEntry( 0, line,  _( "Local Time" ));
  thetime = a_red( getTimeFromJD( date->getJD()) + loc->getTimeZone() + loc->getDST(), 24 );
	table->setEntry( 1, line++, f->getTimeFormatted( thetime ));

	table->setEntry( 0, line,  _( "Location" ));
	table->setEntry( 1, line++, loc->getLocName() );

	table->setEntry( 0, line,  _( "Longitude" ));
	table->setEntry( 1, line++, f->getLongitudeFormatted( loc->getLongitude()));

	table->setEntry( 0, line,  _( "Latitude" ));
	table->setEntry( 1, line++, f->getLatitudeFormatted( loc->getLatitude()));

	table->setEntry( 0, line,  _( "Time Zone" ));
	double tz = loc->getTimeZone();
  s.Printf( wxT( "%s %c%.1f" ), _( "UT" ), ( tz >= 0 ? '+' : '-' ), tz );
	table->setEntry( 1, line++, s );

	table->setEntry( 0, line,  _( "DST" ));
  s.Printf( wxT( "%.1f" ), loc->getDST());
	table->setEntry( 1, line++, s );

	table->setEntry( 0, line,  _( "UT" ));
	table->setEntry( 1, line++, f->getTimeFormatted( getTimeFromJD( h->getJD() )));
}

/*****************************************************
**
**   PrintoutHelper   ---   writeWesternUranian
**
******************************************************/
void PrintoutHelper::writeWesternUranian()
{
	int i;
	wxString s;
	table->setHeader( 1,  _( "Base Data" ));
	table->setHeader( 4,  _( "Planets" ));
	table->setHeader( 9,  _( "Uranian" ));

	int line = 1;
	vector<int> obs = PlanetList().getWesternObjectList( OBJECTS_INCLUDE_OUTER );
  for( unsigned int p = 0; p < obs.size(); p++ )
		writePlanetaryObject( 2, line++, obs[p], true, false );

	line = 1; // next line
	table->col_line[6] = true;
	writePlanetaryObject( 7, line++, OASCENDANT, false, false );
	writePlanetaryObject( 7, line++, OMERIDIAN, false, false );
	for(  i = OCUPIDO; i <= OPOSEIDON; i++ ) writePlanetaryObject( 7, line++, i, true, false );
}

/*****************************************************
**
**   PrintoutHelper   ---   writeWesternDefault
**
******************************************************/
void PrintoutHelper::writeWesternDefault()
{
	int line = 1;
	int col = 2;
	table->setHeader( 4,  _( "Planets" ));
	table->col_line[6] = true;

	vector<int> obs = PlanetList().getWesternObjectList( OBJECTS_INCLUDE_OUTER );
	if ( config->wLunarNodeMode == LUNAR_NODE_TRUE ) obs.push_back( OTRUEDESCNODE );
	else obs.push_back( OMEANDESCNODE );

  for( unsigned int p = 0; p < obs.size(); p++ )
	{
		if ( p == 9 )
		{
			col = 7;
			line = 1;
		}
		writePlanetaryObject( col, line++, obs[p], true, false );
	}
	writePlanetaryObject( col, line++, OASCENDANT, false, false );
	writePlanetaryObject( col, line++, OMERIDIAN, false, false );
	writeHouse( col, line++, 11, false );
	writeHouse( col, line++, 12, false );
	writeHouse( col, line++, 2, false );
	writeHouse( col, line++, 3, false );
}

/*****************************************************
**
**   PrintoutHelper   ---   writePlanetaryObject
**
******************************************************/
void PrintoutHelper::writePlanetaryObject( const int& row, const int &line, const int& i, const bool& withHouse, const bool &vedic, const bool cutname )
{
	wxString s;
	Formatter *formatter = Formatter::get();
	ObjectPosition pos = h->getObjectPosition( i, vedic );

	table->setEntry( row, line, writer->getObjectName( i, TLARGE, true ), config->usePlanetSymbols );
	if ( pos.retro ) table->setEntry( row + 1, line, ( config->usePlanetSymbols ? wxT( "_" ) : wxT( "R" )), config->usePlanetSymbols );

	if ( config->useVedicPositions )
		table->setEntry( row + 2, line, writer->getPosFormatted( pos.length, pos.retro, DEG_PRECISION_SECOND ), false );
	else
	{
		table->setEntry( row + 2, line, formatter->getLenFormatted( getRasiLen( pos.length )), false );
		table->setEntry( row + 3, line, writer->getSignName( getRasi( pos.length ), cutname ? TSHORT : TMEDIUM ), config->useSignSymbols );
	}
	if ( withHouse )
	{
		s.Printf( wxT( "%02d" ), h->getHousePos( i, vedic ) + 1 );
		table->setEntry( row + 4, line, s );
	}
}

/*****************************************************
**
**   PrintoutHelper   ---   writeHouse
**
******************************************************/
void PrintoutHelper::writeHouse( const int &row, const int &line, const int& house, const bool &vedic )
{
	int lord, sublord;
	double len;
	wxString s;
	Lang lang;
	Formatter *formatter = Formatter::get();

	if ( config->useVedicPlanetNames ) s = lang.getBhavaName( house - 1 );
	else s.Printf( wxT( "%02d" ), house );
	table->setEntry( row, line, s );

	len = h->getHouse( house - 1, vedic );
	if ( config->useVedicPositions )
		table->setEntry( row + 2, line, writer->getPosFormatted( len, false, DEG_PRECISION_SECOND ), false );
	else
	{
		table->setEntry( row + 2, line, formatter->getLenFormatted( getRasiLen( len ), DEG_PRECISION_MINUTE ));
		table->setEntry( row + 3, line, writer->getSignName( getRasi( len )), config->useSignSymbols );
	}
	if ( vedic )
	{
		h->getHouseKPLords( house - 1, &lord, &sublord );

		table->setEntry( row + 5, line, writer->getObjectName( lord, TSHORT, true ),
			config->usePlanetSymbols );
		table->setEntry( row+ 6, line, writer->getObjectName( sublord, TSHORT, true ),
			config->usePlanetSymbols );
	}
}

/*****************************************************
**
**   PrintoutHelper   ---   writeVedicDefault
**
******************************************************/
void PrintoutHelper::writeVedicDefault()
{
	int i;
	Lang lang;

	VargaHoroscope chart( h, 0 );
	VargaHoroscope nchart( h, 1 );
	JaiminiExpert jexpert( &nchart );
	jexpert.calcCharaKarakas();
  NakshatraExpert nexpert;

	table->setHeader( 4,  _( "Length" ));
	table->setHeader( 6,  _( "Nakshatra" ));
	table->setHeader( 7,  _( "D-9" ));
	table->setHeader( 8,  _( "Karaka" ));
	table->col_line[5] = true;
  int line = 1;

	vector<int> obs = PlanetList().getVedicObjectList( OBJECTS_INCLUDE_ASCENDANT | OBJECTS_INCLUDE_DRAGONTAIL | OBJECTS_INCLUDE_DRAGONHEAD );
  for( unsigned int p = 0; p < obs.size(); p++ )
  {
		i = obs[p];
		writePlanetaryObject( 2, line, i, false, true );
		table->setEntry( 6, line, lang.getNakshatraName(::getNakshatra(h->getObjectPosition( i, true ).length, N27), N27, TLARGE ) );
    table->setEntry( 7, line, writer->getSignName(nchart.getRasi( i ), config->signPrecision ), config->useSignSymbols );
    if ( p < 8 )  table->setEntry( 8, line, lang.getKarakaName( jexpert.getCharaKarakaProperty(p)) );
    line++;
  }
}

/*****************************************************
**
**   PrintoutHelper   ---   writeVedicKp
**
******************************************************/
void PrintoutHelper::writeVedicKp()
{
	int i, lord, sublord;
	wxString s;
	Lang lang;

	VargaHoroscope chart( h, 0 );
	VargaHoroscope nchart( h, 1 );
	JaiminiExpert jexpert( &nchart );
	jexpert.calcCharaKarakas();
  NakshatraExpert nexpert;
	h->updateKP( 0 );

	table->setHeader( 4,  _( "Length" ));
	table->setHeader( 6,  _( "Naks" ));
	table->setHeader( 7,  _( "L" ));
	table->setHeader( 8,  _( "SL" ));
	//table->setHeader( 9,  _( "House" ));
	table->col_alignment[9] = Align::Right;

	table->setHeader( 11,  _( "Cusp" ));
	table->col_alignment[11] = Align::Right;
	table->setHeader( 14,  _( "L" ));
	table->setHeader( 15,  _( "SL" ));
	table->col_line[8] = true;
	int line = 1;

	vector<int> obs = PlanetList().getVedicObjectList( OBJECTS_INCLUDE_ASCENDANT | OBJECTS_INCLUDE_DRAGONTAIL | OBJECTS_INCLUDE_DRAGONHEAD );
  for( unsigned int p = 0; p < obs.size(); p++ )
  {
		i = obs[p];
		writePlanetaryObject( 2, line, i, false, true, true );
		table->setEntry( 6, line, lang.getNakshatraName(::getNakshatra(h->getObjectPosition( i, true ).length, N27), N27, TMEDIUM ) );

		h->getKPLords( i, &lord, &sublord );
		table->setEntry( 7, line, writer->getObjectName( lord, TSHORT, true ),
			config->usePlanetSymbols );
		table->setEntry( 8, line, writer->getObjectName( sublord, TSHORT, true ),
			config->usePlanetSymbols );
		line++;
  }
	line = 1;
	for( i = 2; i <= 12 ; i++ )
	{
		writeHouse( 9, line++, i, true );
	}
}

/*****************************************************
**
**   PrintoutHelper   ---   writePrintoutSummary
**
******************************************************/
void PrintoutHelper::writePrintoutSummary( const int &st )
{
	style = st;
	int num_cols, num_rows = 30;	
	switch( style )
	{
		case PSUM_WDEFAULT:
			num_rows = 10;
			num_cols = 12;
		break;
		case PSUM_WURANIAN:
			num_rows = 12;
			num_cols = 12;
		break;
		case PSUM_VDEFAULT:
			num_rows = 11;
			num_cols = 9;
		break;
		case PSUM_VKP:
			num_rows = 12;
			num_cols = 16;
		break;
		default:
			assert( false );
		break;
	}

	table = new Table( num_cols, num_rows );
	table->col_line[1] = true;
	table->col_alignment[0] = Align::Right;
	table->col_alignment[4] = Align::Right;
	table->setHeader( 0,  wxT( "" ));
	table->setHeader( 1,  wxT( "" ));

	writeBaseData();
	switch( style )
	{
		case 0:
			writeWesternDefault();
		break;
		case 1:
			writeWesternUranian();
		break;
		case 2:
			writeVedicDefault();
		break;
		case 3:
			writeVedicKp();
		break;
		default:
			assert( 0 );
		break;
	}
	writer->writeTable( *table ); 
}



