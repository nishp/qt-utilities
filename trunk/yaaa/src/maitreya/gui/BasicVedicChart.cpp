/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/BasicVedicChart.cpp
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
	#pragma implementation "BasicVedicChart.h"
#endif

#include "BasicVedicChart.h"

#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "Varga.h"
#include "VedicChartConfig.h"
#include "Writer.h"

#include <math.h>

extern Config *config;

/*****************************************************
**
**   ChartContents   ---   clear
**
******************************************************/
void ChartContents::clear()
{
	textitems.clear();
	graphicitems.clear();
	planets.Clear();
}

/*****************************************************
**
**   BasicVedicChart   ---   Constructor
**
******************************************************/
BasicVedicChart::BasicVedicChart( const ChartProperties *chartprops, const int &field_count, const int chart_count )
 : 	chartprops( chartprops ),
	field_count( field_count )
{
	this->chart_count = chart_count;
	assert( chartprops );
	assert( field_count == 12 || field_count == 28 );
	assert( chart_count == 1 || chart_count == 2 );

	for( int i = 0; i < field_count; i++ )
	{
		contents.push_back( ChartContents() ); 
		tcontents.push_back( ChartContents() ); 
	}

	ptunit = false;
	text_writer = WriterFactory().getWriter( WRITER_TEXT );

	vconf = (VedicChartConfig*)VedicChartConfigLoader::get()->getConfig( chartprops->getVedicSkin() );
}

/*****************************************************
**
**   BasicVedicChart   ---   Destructor
**
******************************************************/
BasicVedicChart::~BasicVedicChart()
{
	delete text_writer;
}

/*****************************************************
**
**   BasicVedicChart   ---   OnDataChanged
**
******************************************************/
void BasicVedicChart::OnDataChanged()
{
  writeChartContents( 0 );
  if ( chart_count == 2 ) writeChartContents( 1 );
}

/*****************************************************
**
**   BasicVedicChart   ---   writeChartContents
**
******************************************************/
void BasicVedicChart::writeChartContents( const int &chart_id )
{
	int planet, field;
	wxString lname, sname, symbol;
	bool retro;
	Lang lang;

	if ( chartprops->isBlank()) return;

	vector<ChartContents> &c = getChartContents( chart_id );
  for( unsigned int i = 0; i < c.size(); i++ ) c[i].clear();

	for( unsigned int i = 0; i < chartprops->getVedicPlanetList().size(); i++ )
	{
		planet = chartprops->getVedicPlanetList()[i];

		// Skip Ascendant for north Indian chart
		// TODO Was ist mit SBC??
		if ( planet == OASCENDANT && chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN ) continue;

		field = getPlanetField( planet, chart_id );
		c[field].planets.Add( planet );

		lname = text_writer->getObjectName( planet, TLARGE, true );
		sname = text_writer->getObjectName( planet, TMEDIUM, true );

		// retrogression
		retro = ( chartprops->getVedicGraphicStyle() & VGRAPHIC_SHOW_RETRO ) && getPlanetRetro( planet, chart_id )
			&& planet != OMEANNODE && planet != OMEANDESCNODE;
		if ( retro )
		{
			lname += wxT( " " );
			lname += _( "(R)" );
			sname += _( "R" );
		}

		// symbol
		symbol.Clear();
		if ( config->usePlanetSymbols && vconf->useSymbols && planet <= MAX_EPHEM_OBJECTS ) symbol = lang.getPlanetSymbolCode( planet );
		if ( ! symbol.IsEmpty() ) c[field].graphicitems.push_back( ChartGraphicItem( symbol, planet, retro ));
		else
		{
			ChartTextItem item( lname, sname, retro );
			c[field].textitems.push_back( item );
		}
	}
}

/*****************************************************
**
**   BasicVedicChart   ---   doPaint 
**
******************************************************/
void BasicVedicChart::doPaint( Painter *painter, const wxRect &r, const wxRect *rrect )
{ 
  assert( painter );
	this->painter = painter;
	refreshRect = rrect;
	vconf = (VedicChartConfig*)VedicChartConfigLoader::get()->getConfig( chartprops->getVedicSkin() );

	xmax = r.width;
	ymax = r.height;

	xcenter = r.x + xmax/2;
	ycenter = r.y + ymax/2;
	xr = (int)(xmax * 0.00475 * vconf->outerZodiac);
	yr = (int)(ymax * 0.00475 * vconf->outerZodiac);

	symbolzoom =  Min( xmax, ymax );
	symbolzoom /= ( field_count == 12 ? 4 : 6 );

	// TODO: factor 2.8 is too small, why?, see WesternChart::paint
	//if ( ptunit ) symbolzoom *= 2.834;
	if ( ptunit ) symbolzoom *= 5;

	painter->getTextExtent( wxT( "a" ), &text_width,  &text_height );

	painter->setPenColor( vconf->fgColor );
	painter->setTextColor( vconf->textColor );

	if ( ! chartprops->isBlank() ) paintChart();
	painter->setPenColor( config->fgColor );
	painter->setTextColor( config->fgColor );
}

/*****************************************************
**
**   BasicVedicChart   ---   getChartContents
**
******************************************************/
vector<ChartContents> &BasicVedicChart::getChartContents( const int &chart_id )
{
	assert( chart_id == 0 || chart_id == 1 );
	return ( chart_id ? tcontents : contents );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawFieldText 
**
******************************************************/
void BasicVedicChart::drawFieldText( const double &x, const double &y, const double &w, const double &h,
	const int &r, const int& align, const int &chart_id )
{
	drawFieldText( wxRect( (int)x, (int)y, (int)w, (int)h ), r, align, chart_id );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawFieldText
**
******************************************************/
void BasicVedicChart::drawFieldText( wxRect rect, const int &r, const int& align, const int &chart_id, const int border )
{
	wxString s;

	vector<ChartContents> &c = getChartContents( chart_id );

	//painter->drawRectangle( rect );

	if ( border > 0 )
	{
		if ( align & Align::Left )
		{
			rect.x += border;
			rect.width -= border;
		}
		else if ( align & Align::Right )
		{
			rect.width -= border;
		}
		if ( align & Align::Top )
		{
			rect.y += border;
			rect.height -= border;
		}
		else if ( align & Align::Bottom )
		{
			rect.height -= border;
		}
	}

	assert( (int)c.size() > r );

	unsigned int titems = c[r].textitems.size();
	unsigned int gitems = c[r].graphicitems.size();

	if ( titems == 0 && gitems == 0 ) return; // nothing to do

	int max_lines = (int)Max(rect.height/ text_height, 1 );  // Maximum number of lines for complete contents

	double gitems_pline = 1;
	double titems_pline = 1;
	int count = 0;
	int total_lines = (int)(ceil( gitems/gitems_pline ) + ceil( titems/titems_pline ));
	while ( total_lines > max_lines && count++ < 40 )
	{
		gitems_pline++;
		titems_pline++;
		total_lines = (int)(ceil( gitems/gitems_pline ) + ceil( titems/titems_pline ));
	}
	assert( count < 400 );

	int total_height = (int)(total_lines * text_height);
	int starty = rect.y;
	if ( align & Align::Bottom ) starty = rect.y + rect.height - total_height;
	else if ( align & Align::VCenter ) starty = rect.y + ( rect.height - total_height ) / 2;

	wxRect trect = rect;
	trect.y = starty;
	trect.height = (int)text_height;

	// GRAPHIC ITEMS
	vector<ChartGraphicItem> g;
	unsigned int i = 0;
	int j = 0;
	painter->setSymbolFont( symbolzoom );
	while( i < gitems )
	{
		g.push_back( c[r].graphicitems[i++] );
		if ( ++j >= gitems_pline )
		{
			drawGraphicItemLine( trect, g, align );
			j = 0;
			trect.y += (int)text_height;
			g.clear();
		}
	}
	if ( g.size() > 0 )
	{
		drawGraphicItemLine( trect, g, align );
		trect.y += (int)text_height;
	}

	// TEXT ITEMS
	painter->setGraphicFont();
	i = 0;
	j = 0;
	s.Clear();
	while( i < titems )
	{
		if ( s.Len() > 0 ) s << wxT( " " );

		if ( titems_pline == 1
			&& c[r].textitems[i].longname.Len() * text_width < rect.width )
		{
			s << c[r].textitems[i++].longname;
		}
		else s << c[r].textitems[i++].shortname;

		/*
		if ( titems_pline == 1 ) s << c[r].textitems[i++].longname;
		else s << c[r].textitems[i++].shortname;
		*/
		
		if ( ++j >= titems_pline )
		{
			drawTextItemLine( trect, s, align );
			j = 0;
			trect.y += (int)text_height;
			s.Clear();
		}
	}
	if ( s.Len() > 0 ) drawTextItemLine( trect, s, align );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawSingleGraphicItem
**
******************************************************/
void BasicVedicChart::drawSingleGraphicItem( const wxRect &rect, const ChartGraphicItem &g )
{
	int cindex;
	//painter->drawRectangle( rect );

	if ( vconf->useTextColors && chartprops->getVedicGraphicStyle() & VGRAPHIC_SHOW_PLANET_COLORS )
	{
		cindex = g.pindex;
		if ( g.pindex == OASCENDANT ) cindex = 0;
		else if ( g.pindex == OMERIDIAN ) cindex = 1;
		painter->setTextColor( cindex <= MAX_EPHEM_OBJECTS ? vconf->getPlanetColor( cindex ) : vconf->fgColor );
	}
	painter->drawSimpleText( rect.x, rect.y, rect.width, rect.height, g.name );
	if ( g.retro ) painter->drawSimpleText( rect.x + text_width, rect.y+text_height/3, rect.width, rect.height, wxT( "_" ));
}

/*****************************************************
**
**   BasicVedicChart   ---   drawTextItemLine
**
******************************************************/
void BasicVedicChart::drawTextItemLine( wxRect rect, const wxString &s, const int &align )
{
	double h, w;
	painter->setTextColor( vconf->fgColor );
	painter->getTextExtent( s, &w, &h );

	if ( align & Align::Right ) rect.x = (int)(rect.x + rect.width - w);
	else if ( align & Align::HCenter ) rect.x = (int)(rect.x + ( rect.width - w ) / 2);

	//painter->drawRectangle( rect );
	painter->drawTextFormatted( rect, s, Align::Left + Align::Top );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawGraphicItemLine
**
******************************************************/
void BasicVedicChart::drawGraphicItemLine( wxRect rect, const vector<ChartGraphicItem> g, const int &align )
{
	int leftx = rect.x;
	int xstep = (int)(2 * text_width);
	int total_width = g.size() * xstep;

	if ( align &  Align::Right ) leftx = rect.x + rect.width - total_width;
	else if ( align & Align::HCenter ) leftx = rect.x + ( rect.width - total_width ) / 2;

	wxRect trect = rect;
	trect.x = leftx;
	trect.width = xstep;
	for( unsigned int i = 0; i < g.size(); i++ )
	{
		drawSingleGraphicItem( trect, g[i] );
		trect.x += xstep;
	}
}

/*****************************************************
**
**   BasicVedicChart   ---   drawTextFormatted
**
******************************************************/
void BasicVedicChart::drawTextFormatted( const wxRect &rect, const wxString t, const int& align )
{
	painter->drawTextFormatted( rect, t, align );
}


