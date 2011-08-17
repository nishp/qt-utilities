/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/DiagramWidget.cpp
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
	#pragma implementation "DiagramWidget.h"
#endif

#include "DiagramWidget.h"

#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Lang.h"
#include "Painter.h"
#include "Writer.h"

#include<math.h>

extern Config *config;

IMPLEMENT_CLASS( DiagramWidget, BasicWidget )

/*****************************************************
**
**   DiagramInfo   ---   addItem
**
******************************************************/
void DiagramInfo::addItem( const DiagramItem &item )
{
	items.push_back( item );
}

/*****************************************************
**
**   DiagramInfo   ---   clear
**
******************************************************/
void DiagramInfo::clear()
{
	items.clear();
}

/*****************************************************
**
**   DiagramWidget   ---   Constructor 
**
******************************************************/
DiagramWidget::DiagramWidget( wxWindow *parent, ChartProperties *chartprops, const int &cols, const int &rows )
 : BasicWidget( parent, chartprops, -1 ),
	cols( cols ),
	rows( rows )
{
	planetmode = true;
	writer = WriterFactory().getWriter( WRITER_TEXT );
}

/*****************************************************
**
**   DiagramWidget   ---   doPaint
**
******************************************************/
void DiagramWidget::doPaint( bool eraseBackground, const wxRect* rect )
{
	int w1, h1;
	double w, h;

  assert( painter );
	getWidgetSize( &w1, &h1  );
	h = h1;
	w = w1;

	// special treatment of 2/1 diagrams
	if ( cols == 2 && rows == 1 )
	{
		xstep = w / 3;
		ystep = h / 2;
		xdiagramstart = w / 6;
		ydiagramstart = h / 4;
	}
	else
	{
		xstep = w / cols;
		ystep = h / rows;
		xdiagramstart = ydiagramstart = 0;
	}

	painter->setBrushColor( config->colors->bgColor );
	painter->drawRectangle( 0, 0, w, h );

	paintDiagrams(); 
}

/*****************************************************
**
**   DiagramWidget   ---   paintDiagram
**
******************************************************/
void DiagramWidget::paintDiagram( const int &col, const int& row, wxString title )
{
	unsigned int i;
	Lang lang;
	wxString s;
	double dummy, maxvalue = info.xmax, minvalue = 0;

	int x0 = xdiagramstart + col * xstep;
	int y0 = ydiagramstart + row * ystep;
	double width = xstep;
	double height = ystep;

	const double xmargin = 5;
	const double xleft = x0 + xmargin;
	const double xwidth = ( width - 2 * xmargin );

	// calculate maximum value if not set
	if ( maxvalue == 0 )
	{
		for ( i = 0; i < info.items.size(); i++ ) maxvalue = Max( maxvalue, info.items[i].value );
		
		// Find a reasonable value for maxvalue
		maxvalue += maxvalue / 10;
		if ( maxvalue > 40 ) maxvalue = 20 * ceil( maxvalue / 20 );
		else maxvalue = 8 * ceil( maxvalue / 8 );
	}
	// maxvalue = 0 may happen if all values are null (e.g. Yuddha Bala)
	if ( maxvalue == 0 ) maxvalue = 60;

	// xmin handling
	for ( i = 0; i < info.items.size(); i++ )
	{
			minvalue = Min( minvalue, info.items[i].value );
	}
	if ( minvalue > 0 ) minvalue = 0;

	// Total y value
	const double valuespan = maxvalue - minvalue;

	const int deltasteps = 4;

	const double xdmargin = 10;
	const double xdleft = xleft + deltasteps * xdmargin;
	const double xdwidth = ( xwidth - ( 1 + deltasteps ) * xdmargin );
	const double xdright = xdleft + xdwidth;

	const double xstep = xdwidth / info.items.size();
	const double xtol = xstep / 10;

	const double ymargin = 10;
	const double ytop = y0 + ymargin;
	const double yheight = ( height - 2 * ymargin );

	const double ydmargin = 10;
	const double ydtop = ytop + ydmargin;
	const double ydheight = ( yheight - 4 * ydmargin );
	const double ydbottom = ydtop + ydheight;
	const double ystep = ydheight / 4;

	//const double y0line = ( minvalue < 0 ) ? ydbottom : ( ydbottom + ydheight * minvalue / valuespan );
	double y0line = ydbottom;
	if ( minvalue < 0 ) y0line += ydheight * minvalue / valuespan;
	//printf( "y0line is %f minvalue %f otto1 %f otto2 %f\n", y0line, minvalue, ydbottom,  ydbottom + ydheight * minvalue / valuespan );

	x3d = xmargin;
	y3d = ymargin;

	double value, currenty;

  painter->setPenColor( config->colors->fgColor );
  painter->setBrushColor( config->colors->bgColor );
	painter->drawRectangle( xleft, ytop, xwidth, yheight );
	painter->setGraphicFont();
	painter->getTextExtent( wxT( "0" ), &dummy, &text_height );

	painter->drawLine( xdleft, ydbottom, xdright, ydbottom );
	painter->drawLine( xdleft, ydbottom, xdleft, ydtop );

	// draw line at y = 0 if negative bars appear
	if ( minvalue < 0 )
	{
		painter->drawLine( xdleft, y0line, xdright, y0line );
	}

	for( i = 0; i <= 4; i++ )
	{
		s.Printf( wxT( "%d" ), (int)( minvalue + .25 * i * valuespan));
		currenty = ydbottom - i * ystep;
		painter->drawLine( xdleft - ydmargin, currenty, xdleft, currenty );
		painter->drawTextFormatted( xdleft - xstep - ydmargin, currenty - ymargin, xstep, 2 * ymargin,
			s, Align::Right + Align::VCenter );
	}

	painter->drawTextFormatted( xleft, ydtop, xwidth, yheight, title, Align::Top + Align::HCenter );
	for ( i = 0; i < info.items.size(); i++ )
	{
		value = fabs( ydheight * info.items[i].value / valuespan );
		double ybarbottom = ydbottom;

		if ( minvalue < 0 )
		{
			if ( info.items[i].value >= 0 )
			{
				// shift bars for positive values 
				ybarbottom += ydheight * minvalue / valuespan;
			}
			else
			{
				ybarbottom += ydheight * ( minvalue - info.items[i].value ) / valuespan;
			}
		}
		drawBar( xdleft + i * xstep + xtol, ybarbottom, xstep - 2 * xtol, value, info.items[i].objectId, info.items[i].value );
	}
  painter->setPenColor( config->colors->fgColor );
  painter->setTextColor( config->colors->fgColor );
  painter->setTransparentBrush();

	bool symbolmode = ( planetmode ? config->usePlanetSymbols : config->useSignSymbols );
	if ( symbolmode ) painter->setSymbolFont();

	for ( i = 0; i < info.items.size(); i++ )
	{
		if ( symbolmode )
		{
			if ( planetmode ) s = lang.getPlanetSymbolCode( info.items[i].objectId );
			else s = lang.getSignSymbolCode( info.items[i].objectId );
		}
		else
		{
			if ( planetmode ) s = writer->getObjectName( info.items[i].objectId, TMEDIUM );
			else s =	writer->getSignName( info.items[i].objectId, TMEDIUM );
		}
		painter->drawTextFormatted( xdleft + i * xstep , ydbottom, xstep, xstep, s, Align::Top + Align::HCenter );
	}
	painter->setGraphicFont();
}

/*****************************************************
**
**   DiagramWidget   ---   drawBar
**
******************************************************/
void DiagramWidget::drawBar( const double &left, const double &bottom, const double &w, const double &h,
	const int &object, const double &value )
{
	wxString s;

	painter->setDasaColors( object, ! planetmode );

	wxPoint leftRhomb[4] = { wxPoint( (int)(left + w), (int)bottom ), wxPoint( (int)(left + w + x3d), (int)(bottom - y3d )),
		wxPoint( (int)(left + w + x3d), (int)(bottom - h - y3d )), wxPoint( (int)(left + w), (int)(bottom - h )) };
	painter->drawPolygon( 4, leftRhomb );

	wxPoint topRhomb[4] = { wxPoint( (int)(left), (int)(bottom - h) ), wxPoint( (int)(left + x3d), (int)(bottom - h - y3d )),
		wxPoint( (int)(left + w + x3d), (int)(bottom - h - y3d )), wxPoint( (int)(left + w), (int)(bottom - h )) };
	painter->drawPolygon( 4, topRhomb );

	painter->drawRectangle( left, bottom - h, w+1, h );

	s.Printf( wxT( "%d" ), (int)(value + .5) ); // rounded value
	if ( h > text_height ) painter->drawTextFormatted( left, bottom - h, w, h, s, Align::Center );
	else 
	{
		painter->setTextColor( config->colors->fgColor );
		painter->drawTextFormatted( left, bottom - h - 2 * text_height, w, text_height, s, Align::HCenter + Align::Bottom );
	}
}

