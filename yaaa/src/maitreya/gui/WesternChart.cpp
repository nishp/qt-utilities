/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/WesternChart.cpp
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
	#pragma implementation "WesternChart.h"
#endif

#include "WesternChart.h"

#include <math.h>

#include "ArabicParts.h"
#include "Aspect.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "constants.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "Lang.h"
#include "Painter.h"
#include "PlanetList.h"
#include "WesternChartConfig.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   WesternChart   ---   Constructor
**
******************************************************/
WesternChart::WesternChart( const Horoscope *horoscope, const Horoscope* htransit, const ChartProperties *chartprops )
: horoscope( horoscope ),
	htransit( htransit ),
	chartprops( chartprops )
{
	assert( chartprops );
	aexpert = new AspectExpert( horoscope );
	transitmode = ( htransit != horoscope );
	ptunit = false;
 	writer = WriterFactory().getWriter( WRITER_TEXT );
}

/*****************************************************
**
**   WesternChart   ---   Destructor
**
******************************************************/
WesternChart::~WesternChart()
{
	delete aexpert;
	delete writer;
}

/*****************************************************
**
**   WesternChart   ---   paint 
**
******************************************************/
void WesternChart::paint( Painter *painter, const wxRect &rect )
{
	this->painter = painter;
	wconf = (WesternChartConfig*)WesternChartConfigLoader::get()->getConfig( chartprops->getWesternSkin() );
	wxColour cr;
	if ( horoscope == 0 )
	{
		printf( "Horosocpe NULL, return\n" );
		return;
	}
	if ( htransit == 0 )
	{
		printf( "HTransit NULL, return\n" );
		return;
	}

  double rmax = .5 * Min( rect.width, rect.height );
	if ( transitmode ) rmax *= .9;
	xcenter = rect.x  + rect.width / 2;
	ycenter = rect.y + rect.height / 2;

	// set factor for symbol height. 100 will be the default size;
	symbolzoom = rmax / 3;

	// TODO: factor 2.8 is too small, why? See BasicVedicChart::doPaint
	//if ( ptunit ) symbolzoom *= 2.834;
	if ( ptunit ) symbolzoom *= 4;

	rInnerZodiac = rmax * wconf->innerZodiac / 100;
	rZodiacDegree1 = rmax * wconf->zodiacDegree1 / 100;
	rZodiacDegree5 = rmax * wconf->zodiacDegree5 / 100;
	rOuterZodiac = rmax * wconf->outerZodiac / 100;
	rPlanetRadius = rmax * wconf->planetRadius / 100;
	rAspectRadius = rmax * wconf->aspectRadius / 100;
	rInnerHouse = rmax * wconf->innerHouse / 100;
	rOuterHouse = rmax * wconf->outerHouse / 100;
	rOuterAngleMarker = rmax * wconf->outerAngleMarker / 100;
	rInnerAngleMarker = rmax * wconf->innerAngleMarker / 100;

	// Second Chart only
	rCircle2InnerRadius = rmax * wconf->circle2InnerRadius / 100;
	rCircle2Degree1 = rmax * wconf->circle2Degree1 / 100;
	//rCircle2Degree5  = rmax * wconf->circle2Degree5 / 100;
	rCircle2OuterRadius = rmax * wconf->circle2OuterRadius / 100;
	rCircle2PlanetRadius = rmax * wconf->circle2PlanetRadius / 100;

	// TODO
	psize = rmax/20;

	aries = 0;
	if ( horoscope ) 
	{
		if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_ARIES ) aries = 180;
		else if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_CANCER ) aries = 90;
		else if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_LIBRA ) aries = 0;
		else if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_LEFT_CAPRICORN ) aries = 270;
		else aries = red_deg( 180 - horoscope->getHouse( HOUSE1, false ) );  // Ascendant on left side
	}
	painter->setPenColor( wconf->fgColor );
	painter->setTextColor( wconf->textColor );

	paintZodiac();
	if ( ! chartprops->isBlank())
	{
		if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_HOUSES ) paintHouses();
		if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_ASPECTS ) paintAspects();
		paintDummies( horoscope );
		paintAllPlanets( horoscope, rInnerZodiac );
		if ( transitmode ) 
		{
			paintAllPlanets( htransit, rCircle2OuterRadius );
		}
	}
}

/*****************************************************
**
**   WesternChart   ---   OnDataChanged
**
******************************************************/
void WesternChart::OnDataChanged()
{
	if ( htransit && horoscope ) aexpert->update( htransit, chartprops );
}

/*****************************************************
**
**   WesternChart   ---   paintSingleAspect 
**
******************************************************/
void WesternChart::paintSingleAspect( const double &len1, const double &len2, const int &type )
{
	double glen1, glen2, x1, x2, y1, y2;
	wxString s;
	const double lpart1 = .2;
	const double lpart2 = .3;

	glen1 = red_deg( aries + len1 );
	x1 = xcenter + rAspectRadius * cos( glen1 * PI / 180 );
	y1 = ycenter - rAspectRadius * sin( glen1 * PI / 180 );

	glen2 = red_deg( aries + len2 );
	x2 = xcenter + rAspectRadius * cos( glen2 * PI / 180 );
	y2 = ycenter - rAspectRadius * sin( glen2 * PI / 180 );

	if ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_ASPECT_SYMBOLS && type != 0 )
	{
		painter->drawLine( x1, y1, x1 + lpart1 * ( x2 - x1 ), y1 + lpart1 * ( y2 - y1 ));
		painter->drawLine( x1 + lpart2 * ( x2 - x1 ), y1 + lpart2 * ( y2 - y1 ), x2, y2 );

		wxRect rect( x1 + lpart1 * ( x2 - x1 ), y1 + lpart1 * ( y2 - y1 ), ( lpart2 - lpart1 ) * ( x2 - x1 ), ( lpart2 - lpart1 ) * ( y2 - y1 ));
		s = AspectExpert::getAspectSymbol( type );
		if ( s.IsEmpty())
		{
			painter->setGraphicFont();
			//painter->drawRectangle( rect );
			painter->drawTextFormatted( rect, AspectExpert::getAspectShortDescription( type ), Align::Center );
		}
		else
		{
			painter->setSymbolFont( symbolzoom );
			painter->drawTextFormatted( rect, s, Align::Center );
		}
	}
	else
	{
		painter->drawLine( x1, y1, x2, y2 );
	}
	painter->setGraphicFont();
}

/*****************************************************
**
**   WesternChart   ---   paintAspects 
**
******************************************************/
void WesternChart::paintAspects()
{
	assert( htransit );
	assert( horoscope );

	ObjectPosition pos1, pos2;
	list<AspectEvent>::iterator iter;
	const Horoscope *hbase = transitmode ? htransit : horoscope;
	AspectConfigLoader *loader = AspectConfigLoader::get();
	AspectConfig *cfg;
	lastaspect = -1;

	list<AspectEvent> al = aexpert->getAspectList();

	double rInnerMarker = rAspectRadius * ( 1 - wconf->planetMarkerLength / 100 );
	double rOuterMarker = rAspectRadius * ( 1 + wconf->planetMarkerLength / 100 );

	painter->setPenWidth( wconf->circleLineWidth );
	painter->setPenColor( wconf->fgColor );
	painter->drawEllipse( xcenter - rAspectRadius, ycenter - rAspectRadius, 2 * rAspectRadius, 2 * rAspectRadius );
	painter->setPenWidth( 1 );

	for( iter = al.begin(); iter != al.end(); iter++ )
	{
		paintSymLine( aries + hbase->getWesternLength( (*iter).planet1 ), rInnerMarker, rOuterMarker );
		paintSymLine( aries + horoscope->getWesternLength( (*iter).planet2 ), rInnerMarker, rOuterMarker );
	}
	for( iter = al.begin(); iter != al.end(); iter++ )
	{
		cfg = loader->getAspectConfig( (*iter).aspectType );
		painter->setPen( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_ASPECT_COLORS ? cfg->color : wconf->fgColor, cfg->width, convertInt2WxPen( cfg->style ));
		painter->setTextColor( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_ASPECT_COLORS ? cfg->color : wconf->fgColor );

		paintSingleAspect( hbase->getWesternLength( (*iter).planet1 ),
			horoscope->getWesternLength( (*iter).planet2 ), (*iter).aspectType );
	}

	// Reset pen
	painter->setPen( wconf->fgColor, wconf->lineWidth, wxSOLID );
}

/*****************************************************
**
**   WesternChart   ---   getXyCoordinates
**
******************************************************/
void WesternChart::getXyCoordinates( double &x, double &y, const double &len, const double &r )
{
	x = xcenter + r * cos( len * PI / 180 );
	y = ycenter - r * sin( len * PI / 180 );
}

/*****************************************************
**
**   WesternChart   ---   paintHouses 
**
******************************************************/
void WesternChart::paintHouses()
{
	double len, x, y, myrad, houselen;
	wxString s;
	Lang lang;
	const int degOffset = 3;

	painter->setPenColor( wconf->fgColor );
	painter->setPenWidth( wconf->circleLineWidth );

	painter->setPenWidth( wconf->lineWidth );
	if ( wconf->paintInnerHouseCircle )
		painter->drawEllipse( xcenter - rInnerHouse, ycenter - rInnerHouse, 2 * rInnerHouse, 2 * rInnerHouse );

	if ( ! transitmode )
	{
		paintArrow( aries + horoscope->getHouse( HOUSE1, false ), rInnerAngleMarker, rOuterAngleMarker );
		myrad = rOuterAngleMarker;
		len = red_deg( degOffset + aries + horoscope->getHouse( HOUSE1, false ));
		getXyCoordinates( x, y, len, myrad );

		painter->setSymbolFont( symbolzoom );
		if( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_PLANET_COLORS ) painter->setTextColor( wconf->getPlanetColor( OASCENDANT ) );
		painter->drawTextFormatted( wxRect( (int)(x-psize/2), (int)(y-psize/2), (int)(psize), (int)(psize)), 
			lang.getPlanetSymbolCode( OASCENDANT), Align::Center );

		paintArrow( aries + horoscope->getHouse( HOUSE10, false ), rInnerAngleMarker, rOuterAngleMarker );
		len = red_deg( degOffset + aries + horoscope->getHouse( HOUSE10, false ));
		getXyCoordinates( x, y, len, myrad );

		if( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_PLANET_COLORS ) painter->setTextColor( wconf->getPlanetColor( OMERIDIAN ) );
		painter->drawTextFormatted( wxRect( (int)(x-psize/2), (int)(y-psize/2), (int)(psize), (int)(psize)), 
			lang.getPlanetSymbolCode( OMERIDIAN), Align::Center );
	}
	painter->setGraphicFont( symbolzoom );

	painter->setPenColor( wconf->fgColor );
	painter->setTextColor( wconf->fgColor );

	for( int i = HOUSE1; i <= HOUSE12; i++ )
	{
		len = horoscope->getHouse( i, false );
		paintSymLine( aries + len, rInnerHouse, rOuterHouse );
	}

	// House numbers
	if ( wconf->paintHouseNumbers )
	{
		myrad = rInnerHouse + .7 * psize;
		for( int i = HOUSE1; i <= HOUSE12; i++ )
		{
			houselen = red_deg( horoscope->getHouse( i == HOUSE12 ? HOUSE1 : i + 1, false ) - horoscope->getHouse( i, false ));
			len = red_deg( aries + horoscope->getHouse( i, false ) + .5 * houselen );
			s.Printf( wxT( "%d" ), i+1 );
			getXyCoordinates( x, y, len, myrad );
			if( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_HOUSE_COLORS ) painter->setTextColor( wconf->getHouseFgColor( i ) );
			painter->drawTextFormatted( wxRect( (int)(x-psize/2), (int)(y-psize/2), (int)(psize), (int)(psize)), s, Align::Center );
		}
	}
}

/*****************************************************
**
**   WesternChart   ---   getPRadius 
**
******************************************************/
double WesternChart::getPRadius( const int &planet, const Horoscope *h )
{
	unsigned int i;
	int p;
	const int factor = 1;
	double ret = rPlanetRadius;
	if ( transitmode && h == htransit ) ret = rCircle2PlanetRadius;

	double len, dist;
	double mylen = h->getWesternLength( planet );
	
	for( i = 0; i < chartprops->getPlanetList().size(); i++ )
	{
		p = chartprops->getPlanetList()[i];
		if ( p >= planet ) continue;
		if (( p == OASCENDANT || p == OMERIDIAN ) && ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_HOUSES )) continue;
		len = h->getWesternLength( p );
		dist = ::fabs( mylen - len );
		if ( dist > 300 ) dist = fabs( dist - 360 );
		if ( dist < 5 )
		{
			if ( h == htransit ) ret -= Min( factor * psize, factor * psize * ( 5 - dist)/5 );
			else ret += Min( factor * psize, factor * psize * ( 5 - dist)/5 );
		}
	}
	return ret;
}

/*****************************************************
**
**   WesternChart   ---   paintDummies 
**
******************************************************/
void WesternChart::paintDummies( const Horoscope *h )
{
	unsigned int i;
	double len, glen, x, y;
	int planet;
	double mysize = .7 * psize;
	if ( wconf->bgImage.IsEmpty())
	{
		painter->setBrushColor( wconf->bgColor );
		painter->setPenColor( wconf->bgColor );
	}
	else
	{
		// must draw rectangle because stipple must be drawn
		painter->setBackgroundImage( wconf->bgImage );
	}
	painter->setTransparentPen();

	for( i = 0; i < chartprops->getPlanetList().size(); i++ )
	{
		planet = chartprops->getPlanetList()[i];
		if (( planet == OASCENDANT || planet == OMERIDIAN ) && ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_HOUSES )) continue;
		len = horoscope->getWesternLength( planet );
		glen = red_deg( aries + len );
		double r = getPRadius( planet, h );
		getXyCoordinates( x, y, glen, r );
		painter->drawRectangle( x - mysize, y - mysize, 2 * mysize, 2 * mysize );
	}
	painter->setTransparentBrush();
	painter->setPenColor( config->colors->fgColor );
	painter->setBrushColor( wconf->bgColor );
}

/*****************************************************
**
**   WesternChart   ---   getXYValues 
**
******************************************************/
// TODO const nach vorne
void WesternChart::getXYValues( double &x, double &y, const int &planet, const Horoscope *h )
{
	double len = h->getWesternLength( planet );
	if ( planet >= OCHIRON && planet <= OVESTA && len == 0 ) tellThatObjectsAreNotCalculated( 0, 1 );
	double glen = red_deg( aries + len );
	double r = getPRadius( planet, h );
	x = r * cos( glen * PI / 180 );
	y = -r * sin( glen * PI / 180 );
}

/*****************************************************
**
**   WesternChart   ---   paintArrow 
**
******************************************************/
void WesternChart::paintArrow( const double &a, const double &rmin, const double &rmax )
{
	double c = cos( a * PI / 180 );
	double x = rmax * c;
	double s = sin( a * PI / 180 );
	double y = rmax * s;

	if ( rmin == 0 )
		{ painter->drawLine( xcenter-x, ycenter+y, xcenter+x, ycenter-y ); }
	else
	{
		double xx = rmin * c;
		double yy = rmin * s;
		painter->drawLine( xcenter-x, ycenter+y, xcenter-xx, ycenter+yy );
		painter->drawLine( xcenter+x, ycenter-y, xcenter+xx, ycenter-yy );
	}
	double rtemp = rmax * .97;
	double ra = 1;
	double x0, y0;
	x0 = cos( (a - ra) * PI / 180 ) * rtemp;
	y0 = -sin( (a - ra) * PI / 180 ) * rtemp;
	painter->drawLine( xcenter+x, ycenter-y, xcenter+x0, ycenter+y0 );
	x0 = cos( (a + ra) * PI / 180 ) * rtemp;
	y0 = -sin( (a + ra) * PI / 180 ) * rtemp;
	painter->drawLine( xcenter+x, ycenter-y, xcenter+x0, ycenter+y0 );
}

/*****************************************************
**
**   WesternChart   ---   paintSymLine 
**
******************************************************/
void WesternChart::paintSymLine( const double &a, const double &rmin, const double &rmax )
{
	double c = cos( a * PI / 180 );
	double x0 = rmin * c;
	double x1 = rmax * c;
	double s = sin( a * PI / 180 );
	double y0 = rmin * s;
	double y1 = rmax * s;

	painter->drawLine( xcenter+x0, ycenter-y0, xcenter+x1, ycenter-y1 );
}

/*****************************************************
**
**   WesternChart   ---   paintAllPlanets
**
******************************************************/
void WesternChart::paintAllPlanets( const Horoscope *h, const double &markerbase )
{
	int planet, cindex, align;
	double x, y;
	Lang lang;
	double ff = 15;
	wxString psymbol;

	double rInnerMarker = markerbase * ( 1 - wconf->planetMarkerLength / 100 );
	double rOuterMarker = markerbase * ( 1 + wconf->planetMarkerLength / 100 );

	vector<int> obs = chartprops->getWesternPlanetList();
	for( unsigned int i = 0; i < chartprops->getWesternPlanetList().size(); i++ )
	{
		painter->setPenColor( wconf->fgColor );
		painter->setTextColor( wconf->fgColor );
		planet = obs[i];
		if (( planet == OASCENDANT || planet == OMERIDIAN ) && ( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_HOUSES ) && ! transitmode ) continue;
		paintSymLine( aries + h->getWesternLength( planet ), rInnerMarker, rOuterMarker );
		getXYValues( x, y, planet, h );
		wxRect rr( (int)(xcenter+x-ff), (int)(ycenter+y-ff), (int)(2*ff), (int)(2*ff));

		if( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_PLANET_COLORS )
		{
			cindex = planet;
			if ( planet == OASCENDANT ) cindex = 0;
			else if ( planet == OMERIDIAN ) cindex = 1;
			painter->setTextColor( cindex <= MAX_EPHEM_OBJECTS ? wconf->getPlanetColor( cindex ) : wconf->fgColor );
		}

		psymbol = lang.getPlanetSymbolCode( planet );
		if ( psymbol.IsEmpty() )
		{
			painter->setGraphicFont();
			if ( x > 0 && fabs( x ) > fabs( y )) align = Align::Right;
			else if ( x < 0 && fabs( x ) > fabs( y )) align = Align::Left;
			else  align = Align::Center;
			painter->drawTextFormatted( rr, writer->getObjectName( planet, TLARGE ), align );
		}
		else
		{
			painter->setSymbolFont( symbolzoom );
			painter->drawTextFormatted( rr, psymbol, Align::Center );
		}

		// Retrogression
		if (( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_RETRO ) && h->isRetrograde( planet ))
		{
			painter->setSymbolFont( symbolzoom );
			
			align = Align::Right+Align::Bottom;
			painter->drawTextFormatted( wxRect( (int)(xcenter+x), (int)(ycenter+y+psize/2), (int)(psize), (int)(psize)),
				wxT( "_" ), align );
		}
	}
	painter->setGraphicFont();
}

/*****************************************************
**
**   WesternChart   ---   paintZodiac
**
******************************************************/
void WesternChart::paintZodiac()
{
	int i;
	double x, y;
	double a, midrad;

	painter->setPenColor( wconf->fgColor );
	painter->setPenWidth( wconf->circleLineWidth );

	if ( ! wconf->bgImage.IsEmpty() && ! wconf->doFloodFill ) painter->setBackgroundImage( wconf->bgImage );
	painter->drawEllipse( xcenter - rOuterZodiac, ycenter - rOuterZodiac, 2 * rOuterZodiac, 2 * rOuterZodiac );
	painter->setTransparentBrush();
	painter->drawEllipse( xcenter - rInnerZodiac, ycenter - rInnerZodiac, 2 * rInnerZodiac, 2 * rInnerZodiac );

	if ( wconf->paint1DegCircle ) 
		painter->drawEllipse( xcenter - rZodiacDegree1, ycenter - rZodiacDegree1, 2 * rZodiacDegree1, 2 * rZodiacDegree1 );
	if ( wconf->paint5DegCircle ) 
		painter->drawEllipse( xcenter - rZodiacDegree5, ycenter - rZodiacDegree5, 2 * rZodiacDegree5, 2 * rZodiacDegree5 );
	painter->setPenWidth( wconf->lineWidth );

	a = aries;
	for( i = 0; i < 12; i++ )
	{
		paintSymLine( a, rInnerZodiac, rOuterZodiac );
		a += 30;
	}
	a = aries;
	for( i = 1; i < 72; i++ ) { if ( i % 6 ) paintSymLine( a + i * 5, rInnerZodiac, rZodiacDegree5 ); }
	for( i = 1; i < 360; i++ ) { if ( i % 5 ) paintSymLine( a + i, rInnerZodiac, rZodiacDegree1 ); }
	if ( transitmode )
	{
		painter->drawEllipse( xcenter - rCircle2OuterRadius, ycenter - rCircle2OuterRadius, 2 * rCircle2OuterRadius, 2 * rCircle2OuterRadius );
		for( i = 1; i < 72; i++ ) { paintSymLine( a + i * 5, rCircle2InnerRadius, rCircle2OuterRadius ); }
		for( i = 1; i < 360; i++ ) { if ( i % 5 ) paintSymLine( a + i, rCircle2Degree1, rCircle2OuterRadius ); }
	}

	a = aries + 15;

	midrad = ( .95 * rOuterZodiac );

	if ( wconf->symbolType != 2 ) painter->setSymbolFont( symbolzoom * wconf->symbolZoomFactor / 100 );
	for( i = 0; i < 12; i++ )
	{
		getXyCoordinates( x, y, a, midrad );
		if( chartprops->getWesternGraphicStyle() & WGRAPHIC_SHOW_SIGN_COLORS )
		{
			painter->setTextColor( wconf->getSignFgColor( i ));
		}
		painter->drawSignSymbol( x, y, i, wconf->symbolType, wconf->symbolZoomFactor, wconf->rotateSymbols ? a - 90 : 0 );
		a += 30;
	}

	painter->setTextColor( wconf->fgColor );
	painter->setGraphicFont();

	if ( wconf->paintOuterHouseCircle )
		painter->drawEllipse( xcenter - rOuterHouse, ycenter - rOuterHouse, 2 * rOuterHouse, 2 * rOuterHouse );
}

