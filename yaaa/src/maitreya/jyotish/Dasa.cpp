/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Dasa.cpp
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
	#pragma implementation "Dasa.h"
#endif

#include "Dasa.h"

#include <wx/string.h>

#include "astrobase.h"
#include "constants.h"
#include "Horoscope.h"
#include "Lang.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

/*****************************************************
**
**   DasaImpl   ---   Constructor 
**
******************************************************/
DasaImpl::DasaImpl( DasaExpert *e, const int lrd, const double stjd, const double ejd, const wxChar *etext, Dasa *prt )
{
	extraText = etext;
	expert = e;
	lord = lrd;
	start_jd = stjd;
	end_jd = ejd;
	parent = prt;
}

/*****************************************************
**
**   DasaImpl   ---   dump 
**
******************************************************/
void DasaImpl::dump(wxString &o)
{
	Formatter *f = Formatter::get();
 	Writer *writer = WriterFactory().getWriter( WRITER_TEXT );

	if ( expert->isRasiDasaExpert() )
		o << writer->getSignName( lord );
	else
		o <<  writer->getObjectName( lord,TMEDIUM, true );

	o	<< wxT( " " ) << f->getDateStringFromJD( getStartJD() ) << wxT( " - " ) << f->getDateStringFromJD( getEndJD() ) << Endl;
}

/*****************************************************
**
**   RasiDasaImpl   ---   Constructor 
**
******************************************************/
RasiDasaImpl::RasiDasaImpl( DasaExpert *expert, VargaHoroscope *v, const int &lrd, const double &stjd, const double &ejd, const wxChar *etext,
	Dasa *prt )
	: DasaImpl( expert, lrd, stjd, ejd, etext, prt )
{
	chart = new VargaHoroscope( v->getHoroscope(), v->getDivision());
}

/*****************************************************
**
**   RasiDasaImpl   ---   Destructor 
**
******************************************************/
RasiDasaImpl::~RasiDasaImpl()
{
	delete chart;
}

/*****************************************************
**
**   DasaExpert   ---   write
**
******************************************************/
void DasaExpert::write( Writer *writer, const Horoscope *horoscope, const int varga, const bool show_header )
{
	Formatter *formatter = Formatter::get();
	double startjd;
	unsigned int i;
	wxString s, lord;
 
	VargaExpert vexpert;
	if ( show_header )
	{
		s.Printf( wxT( "%s %s" ), getName(), _( "Dasa" ) );
		writer->writeHeader2( s );
	}

	vector<Dasa*> v = getFirstLevel( horoscope, varga );
	vector<Dasa*> w;

	for( i = 0; i < v.size(); i++ )
	{
		if ( isRasiDasaExpert()) lord = writer->getSignName( v[i]->getDasaLord(), TLARGE );
		else lord =  writer->getObjectName(v[i]->getDasaLord(), TLARGE, true );

		startjd = Max( v[i]->getStartJD(), horoscope->getJD() );
		if ( ! v[i]->getExtraText().IsEmpty())
		{
			s.Printf( wxT( "%s (%s): %s - %s" ), (const wxChar*)lord, (const wxChar*)(v[i]->getExtraText()),
				(const wxChar*)formatter->getDateStringFromJD( startjd ),
				(const wxChar*)formatter->getDateStringFromJD( v[i]->getEndJD() ));
		}
		else
		{
			s.Printf( wxT( "%s: %s - %s" ), (const wxChar*)lord,
				(const wxChar*)formatter->getDateStringFromJD( startjd ),
				(const wxChar*)formatter->getDateStringFromJD( v[i]->getEndJD() ));
		}
		writer->writeHeader3( s );
		s.Clear();

		w = getNextLevel( v[i] );
		if ( w.size() == 0 ) continue;
		
		startjd = Max( w[0]->getStartJD(), horoscope->getJD() );
		s << wxT( " * " ) << formatter->getDateStringFromJD( startjd ) << wxT( " " );

		for( unsigned int j = 0; j < w.size(); j++ )
		{
			if ( w[j]->getEndJD() >= horoscope->getJD())
			{
				if ( isRasiDasaExpert() ) 
				{
					s << writer->getSignName(w[j]->getDasaLord(), TLARGE );
				}
				else
				{
					s <<  writer->getObjectName(w[j]->getDasaLord(), TLARGE, true );
				}
				s	<< wxT( " " ) << formatter->getDateStringFromJD( w[j]->getEndJD() ) << wxT( " " );
			}
			delete w[j];
		}
		writer->writeLine( s );
		s.Clear();
	}
	for( i = 0; i < v.size(); i++ ) delete v[i];
}

/*****************************************************
**
**   DasaExpert   ---   writeMore
**
******************************************************/
void DasaExpert::writeMore( Writer *writer, const Horoscope *horoscope, const int varga, const bool show_header )
{
	Formatter *formatter = Formatter::get();
	double startjd;
	unsigned int i;
	wxString s, s1, lord;
 
	VargaExpert vexpert;
	if ( show_header )
	{
	s.Printf( wxT( "%s %s" ), getName(), _( "Dasa" ) );
  writer->writeHeader1( s );
	}

	vector<Dasa*> v = getFirstLevel( horoscope, varga );
	vector<Dasa*> w;

	for( i = 0; i < v.size(); i++ )
	{
		if ( isRasiDasaExpert() ) lord = writer->getSignName( v[i]->getDasaLord(), TLARGE );
		else lord =  writer->getObjectName(v[i]->getDasaLord(), TLARGE, true );
		s.Printf( wxT( "%d. %s %s" ), i+1, _( "Mahadasa" ), (const wxChar*)lord );
		writer->writeHeader2( s );

		if ( ! v[i]->getExtraText().IsEmpty())
			{ writer->writeLine( v[i]->getExtraText() ); }

		startjd = Max( v[i]->getStartJD(), horoscope->getJD() );
		s.Printf( wxT( "%s: %s" ), _( "Start Date" ), (const wxChar*)formatter->getDateStringFromJD( startjd ) );
		writer->writeLine( s );
		s.Printf( wxT( "%s: %s" ), _( "End Date" ), (const wxChar*)formatter->getDateStringFromJD( v[i]->getEndJD() ) );
		writer->writeLine( s );
		getDasaDuration( s1, v[i]->getEndJD() - v[i]->getStartJD() );
		s.Printf( wxT( "%s: %s" ), _( "Duration" ), (const wxChar*)s1 );
		writer->writeLine( s );

		w = getNextLevel( v[i] );
		if ( w.size() == 0 ) continue;
		
		writer->writeHeader3( _( "Antardasas" ) );

		// Get the number of lines
		int lines = 1;
		for( unsigned int j = 0; j < w.size(); j++ )
		{
			if ( w[j]->getEndJD() >= horoscope->getJD()) lines++;
		}

		Table table( 4, lines );
		table.setHeader( 0, _( "Lord" ));
		table.setHeader( 1,  _( "Start Date" ));
		table.setHeader( 2,  _( "Final Date" ));
		table.setHeader( 3,  _( "Duration" ));

		int line = 1;
		for( unsigned int j = 0; j < w.size(); j++ )
		{
			if ( w[j]->getEndJD() >= horoscope->getJD())
			{
				if ( isRasiDasaExpert() ) lord = writer->getSignName(w[j]->getDasaLord(), TLARGE );
				else lord = writer->getObjectName(w[j]->getDasaLord(), TLARGE, true );
				table.setEntry( 0, line, lord );

				startjd = Max( w[j]->getStartJD(), horoscope->getJD() );
				table.setEntry( 1, line, formatter->getDateStringFromJD( startjd ) );
				
				table.setEntry( 2, line, formatter->getDateStringFromJD( w[j]->getEndJD() ) );

				getDasaDuration( s, w[j]->getEndJD() - w[j]->getStartJD() );
				table.setEntry( 3, line, s );
				line++;
			}
			delete w[j];
		}
		writer->writeTable( table );
	}
	for( i = 0; i < v.size(); i++ ) delete v[i];
}

/*****************************************************
**
**   DasaExpert   ---   getKPEventList
**
******************************************************/
vector<KPEvent> DasaExpert::getKPEventList( const double &startlen, const double &Endlen, const double &startjd )
{
	vector<KPEvent> v;
	return v;
}

DasaExpertFactory *DasaExpertFactory::ego = 0;
/*****************************************************
**
**   DasaExpertFactory   ---   Constructor
**
******************************************************/
DasaExpertFactory::DasaExpertFactory()
{
	init();
}

/*****************************************************
**
**   DasaExpertFactory   ---   get
**
******************************************************/
DasaExpertFactory *DasaExpertFactory::get()
{
	if ( ego == 0 ) ego = new DasaExpertFactory();
	return ego;
}

/*****************************************************
**
**   DasaExpertFactory   ---   getDasaExpert
**
******************************************************/
DasaExpert *DasaExpertFactory::getDasaExpert( const unsigned int &i )
{
	assert( i >= 0 && i < experts.size());
	return experts[i];
}

/*****************************************************
**
**   DasaExpertFactory   ---   init
**
******************************************************/
void DasaExpertFactory::init()
{
	experts.clear();
	experts.push_back( createVimsottariDasaExpert());
	experts.push_back( createYoginiDasaExpert());
	experts.push_back( createKalachakraDasaExpert());
	experts.push_back( createAshtottariDasaExpert());
	experts.push_back( createShodshottariDasaExpert());
	
	experts.push_back( createDvadashottariDasaExpert());
	experts.push_back( createPanchottariDasaExpert());
	experts.push_back( createShatabdikaDasaExpert());
	experts.push_back( createChaturashitiSamaDasaExpert());
	experts.push_back( createDvisaptatiSamaDasaExpert());
	experts.push_back( createShatTrimshatSamaDasaExpert());
	experts.push_back( createLagnaVimsottariDasaExpert());

}
