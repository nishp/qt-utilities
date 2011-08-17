/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/Painter.cpp
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
	#pragma implementation "Painter.h"
#endif

#include "Painter.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "FontConfig.h"
#include "IconProvider.h"
#include "Lang.h"
#include "mathbase.h"
#include "FileConfig.h"

#include <wx/dc.h>
#include <wx/filename.h>

extern Config *config;

/*****************************************************
**
**   Painter   ---   drawRectangle
**
******************************************************/
void Painter::drawRectangle( const wxRect &r, const double rnd )
{
	drawRectangle( r.GetLeft(), r.GetTop(), r.GetWidth(), r.GetHeight(), rnd );
}

/*****************************************************
**
**   Painter   ---   drawLine
**
******************************************************/
void Painter::drawLine( const wxPoint &p1, const wxPoint &p2 )
{
	drawLine( p1.x, p1.y, p2.x, p2.y );
}

/*****************************************************
**
**   Painter   ---   setPen
**
******************************************************/
void Painter::setPen( const wxPen &pen )
{
	setPenColor( pen.GetColour());
	setPenWidth( pen.GetWidth() );
	setLineStyle( pen.GetStyle() );
}

/*****************************************************
**
**   Painter   ---   setBrush
**
******************************************************/
void Painter::setBrush( const wxBrush &b )
{
	if ( b.GetStyle() == wxSTIPPLE ) setBackgroundImage( *b.GetStipple());
	else setBrushColor( b.GetColour());
}

/*****************************************************
**
**   Painter   ---   drawSimpleText
**
******************************************************/
void Painter::drawSimpleText( const wxRect &r, const wxString &s )
{
	drawSimpleText( r.GetLeft(), r.GetTop(), r.GetWidth(), r.GetHeight(), s );
}

/*****************************************************
**
**   Painter   ---   drawSimpleText
**
******************************************************/
void Painter::drawSimpleText( const double &x, const double &y, const double &w, const double &h,
	const wxString &t )
{
	drawTextFormatted( wxRect( (int)x, (int)y, (int)w, (int)h ), t, Align::Center );
}

/*****************************************************
**
**   Painter   ---   drawTextFormatted
**
******************************************************/
void Painter::drawTextFormatted( const wxRect &r, const wxString &t, const int& align )
{
	drawTextFormatted( r.GetLeft(), r.GetTop(), r.GetWidth(), r.GetHeight(), t, align );
}

/*****************************************************
**
**   Painter   ---   drawDasaBar
**
******************************************************/
void Painter::drawDasaBar( const int &dasalord, const int &isRasiLord, const DasaColorConfig *cfg, const int &x1, const int &y1,
													const int &w, const int &h, const int &cornerRadius )
{
	int format = TLARGE;
	wxString s;
	Lang lang;
	
	setPen( wxPen( cfg->fgColor, cfg->fgWidth, cfg->fgStyle ));
	setTextColor( cfg->fgColor );
	setBrush( wxBrush( cfg->bgColor, cfg->bgStyle ));
	if ( cfg->bgStyle == wxSTIPPLE && ! cfg->bgImage.IsEmpty() ) setBackgroundImage( *IconProvider::getBitmap( cfg->bgImage ));

	drawRectangle( x1, y1, w, h, cornerRadius );

	if ( w < 80 ) format = TMEDIUM;
	if ( isRasiLord )
	{
			assert( dasalord >= 0 && dasalord < 12 );
			if ( config->useSignSymbols )
			{
				setSymbolFont();
				s = lang.getSignSymbolCode( dasalord );
			}
			else
			{
				setGraphicFont();
				s = lang.getSignName( dasalord, format );
			}
	}
	else // Lord is a planet
	{
			if ( config->usePlanetSymbols )
			{
				setSymbolFont();
				s = lang.getPlanetSymbolCode( dasalord );
			}
			else
			{
				setGraphicFont();
				s = lang.getObjectName( dasalord, format );
			}
	}
	drawTextFormatted( wxRect( x1, y1, w, h ),	s, Align::Center );
}

/*****************************************************
**
**   Painter   ---   setDasaColors
**
******************************************************/
void Painter::setDasaColors( const int &object, const int &rasitype )
{
	setDasaColors( config->colors->getDasaColorConfigId( object, rasitype ));
}

/*****************************************************
**
**   Painter   ---   setDasaColors
**
******************************************************/
void Painter::setDasaColors( const int &objectid )
{
	//DasaColorConfig &dasa = config->colors->getDasaColorConfig( object, rasitype );
	DasaColorConfig &dasa = config->colors->dasa[ objectid];

	setPenColor( dasa.fgColor );
	setPenWidth( dasa.fgWidth );
	setTextColor( dasa.fgColor );
	setBrushColor( dasa.bgColor );
	if ( dasa.bgStyle == wxSTIPPLE && ! dasa.bgImage.IsEmpty() ) setBackgroundImage( *IconProvider::getBitmap( dasa.bgImage ));
}

/*****************************************************
**
**   Painter   ---   doFloodFill
**
******************************************************/
void Painter::doFloodFill( const wxString &bgImage, const wxColour &fgColor, const wxColour &bgColor, const wxRect &rect )
{
	if ( ! bgImage.IsEmpty() )
	{
		if ( supportsBackgroundImages())
		{
			setBackgroundImage( bgImage );
			drawRectangle( rect );
		}
		else {} //tellThatPdfSupportsNoBgImages( 0 );
	}
	else
	{
		setBrushColor( bgColor );
		setPenColor( bgColor );
		drawRectangle( rect );
		setTransparentBrush();
		setPenColor( fgColor );
	}
}

/*****************************************************
**
**   DcPainter   ---   Constructor
**
******************************************************/
DcPainter:: DcPainter( wxDC *d )
{
	dc = d;
}

/*****************************************************
**
**   DcPainter   ---   drawRectangle
**
******************************************************/
void DcPainter::drawRectangle( const double &x, const double &y, const double &w, const double &h, const double rnd )
{
	if ( rnd == 0 ) 
		dc->DrawRectangle( (int)x, (int)y, (int)w, (int)h );
	else
		dc->DrawRoundedRectangle( (int)x, (int)y, (int)w, (int)h, rnd );
}

/*****************************************************
**
**   DcPainter   ---   drawPolygon
**
******************************************************/
void DcPainter::drawPolygon(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	dc->DrawPolygon( n, points, xoffset, yoffset );
}

/*****************************************************
**
**   DcPainter   ---   drawLine
**
******************************************************/
void DcPainter::drawLine( const double &x1, const double &y1, const double &x2, const double &y2 )
{
	dc->DrawLine( (int)x1, (int)y1, (int)x2, (int)y2 );
}

/*****************************************************
**
**   DcPainter   ---   drawTextFormatted
**
******************************************************/
void DcPainter::drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
	const wxString &t, const int& align )
{
	//drawRectangle( x, y, w, h );
	DcHelper( dc ).drawTextFormatted( wxRect( (int)x, (int)y, (int)w, (int)h ), t, align );
}

/*****************************************************
**
**   DcPainter   ---   getTextExtent
**
******************************************************/
void DcPainter::getTextExtent( const wxString &s, double *w, double *h )
{
	wxCoord w1, h1;
	dc->GetTextExtent( s, &w1, &h1 );
	*h = h1; 
	*w = w1;
}

/*****************************************************
**
**   DcPainter   ---   drawEllipse
**
******************************************************/
void DcPainter::drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax )
{
	dc->DrawEllipse( a_rund(x0), a_rund(y0), a_rund(xmax), a_rund(ymax) );
}

/*****************************************************
**
**   DcPainter   ---   drawArc 
**
******************************************************/
void DcPainter::drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
	const double &w1, const double &w2 )
{
	dc->DrawEllipticArc( a_rund(x0), a_rund(y0), a_rund(xmax), a_rund(ymax), a_rund(w1), a_rund( w2) );
}

/*****************************************************
**
**   DcPainter   ---   drawPoint 
**
******************************************************/
void DcPainter::drawPoint( const double &x, const double &y )
{
	dc->DrawPoint( a_rund(x), a_rund(y) );
}

/*****************************************************
**
**   DcPainter   ---   setTransparentBrush
**
******************************************************/
void DcPainter::setTransparentBrush()
{
	wxBrush b = dc->GetBrush();
	b.SetStyle( wxTRANSPARENT );
	dc->SetBrush( b );
}

/*****************************************************
**
**   DcPainter   ---   setBrushColor
**
******************************************************/
void DcPainter::setBrushColor( const wxColour &c )
{
	wxBrush b = dc->GetBrush();
	b.SetStyle( wxSOLID );
	b.SetColour( c );
	dc->SetBrush( b );
}

/*****************************************************
**
**   DcPainter   ---   setBrush
**
******************************************************/
void DcPainter::setBrush( const wxBrush &b )
{
	dc->SetBrush( b );
}

/*****************************************************
**
**   DcPainter   ---   getBrushColor
**
******************************************************/
wxColour DcPainter::getBrushColor()
{
	return dc->GetBackground().GetColour();
}

/*****************************************************
**
**   DcPainter   ---   setTransparentPen
**
******************************************************/
void DcPainter::setTransparentPen()
{
	wxPen p = dc->GetPen();
	p.SetStyle( wxTRANSPARENT );
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   setBackgroundImage
**
******************************************************/
void DcPainter::setBackgroundImage( const wxBitmap &bmp )
{
	wxBrush b = dc->GetBrush();
	b.SetStipple( bmp );
	b.SetStyle( wxSTIPPLE );
	dc->SetBrush( b );
}

/*****************************************************
**
**   DcPainter   ---   setBackgroundImage
**
******************************************************/
void DcPainter::setBackgroundImage( const wxString &s )
{
	wxBrush b = dc->GetBrush();
	b.SetStipple( *IconProvider::getBitmap( s ));
	b.SetStyle( wxSTIPPLE );
	dc->SetBrush( b );
}

/*****************************************************
**
**   DcPainter   ---   setPenColor
**
******************************************************/
void DcPainter::setPenColor( const wxColour &c )
{
	wxPen p = dc->GetPen();
	p.SetStyle( wxSOLID );
	p.SetColour( c );
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   setPenWidth
**
******************************************************/
void DcPainter::setPenWidth( const int &i )
{
	wxPen p = dc->GetPen();
	p.SetWidth( i );
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   setPen
**
******************************************************/
void DcPainter::setPen( const wxColour &color, const int &width, const int &linestyle )
{
	dc->SetPen( *wxThePenList->FindOrCreatePen( color, width, linestyle ));
}

/*****************************************************
**
**   DcPainter   ---   getPenColor
**
******************************************************/
wxColour DcPainter::getPenColor()
{
	return dc->GetPen().GetColour();
}

/*****************************************************
**
**   DcPainter   ---   setLineStyle
**
******************************************************/
void DcPainter::setLineStyle( const int &s )
{
	wxPen p = dc->GetPen();
	p.SetStyle( s );
	//p.SetWidth( 1 );
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   setSymbolFont
**
******************************************************/
void DcPainter::setSymbolFont( const int zoom )
{
	FontConfig *f = FontConfig::get();
	dc->SetFont( *f->getSymbolFont( zoom ));
}

/*****************************************************
**
**   DcPainter   ---   setGraphicFont
**
******************************************************/
void DcPainter::setGraphicFont( const int zoom )
{
	FontConfig *f = FontConfig::get();
	dc->SetFont( *f->getGraphicFont( zoom ));
}

/*****************************************************
**
**   DcPainter   ---   setFontSize
**
******************************************************/
void DcPainter::setFontSize( const int &size )
{
	wxFont font = dc->GetFont();
	font.SetPointSize( size );
	dc->SetFont( font );
}

/*****************************************************
**
**   DcPainter   ---   setTextColor
**
******************************************************/
void DcPainter::setTextColor( const wxColour& c )
{
	dc->SetTextForeground( c );
}

/*****************************************************
**
**   DcPainter   ---   setTextColor
**
******************************************************/
void DcPainter::setTextBackgroundColor( const wxColour &c )
{
	dc->SetTextBackground( c );
}

/*****************************************************
**
**   DcPainter   ---   drawSignSymbol
**
******************************************************/
void DcPainter::drawSignSymbol( const int&x, const int &y, const int &sign, const int type, const int zoom, const double angle )
{
	Lang lang;
	double w, h;
	double xs, ys;
	wxString s;

	if ( type == 2 ) s = lang.getSignName( sign, TSHORT );
	else s = lang.getSignSymbolCode( sign, type );
	getTextExtent( s, &w, &h );

	if ( angle )
	{
		xs = x - .5 * w * cos( angle * DEG2RAD );
		ys = y + .5 * h * sin( angle * DEG2RAD );
	}
	else
	{
		xs = x - .5 * w;
		ys = y - .5 * h;
	}

	double x3 = x - .5 * w * cos( 1.75 * PI + angle * DEG2RAD );
	double y3 = y + .5 * h * sin( 1.75 * PI+ angle * DEG2RAD );

	dc->DrawRotatedText( s, (int)x3, (int)y3, angle );
}

/*****************************************************
**
**   DcPainter   ---   drawSimpleRotatedText
**
******************************************************/
void DcPainter::drawSimpleRotatedText( const wxRect &r, const wxString &s, const double &angle )
{
	wxCoord x, y;
	x = r.x + ( r.width ) / 2;
	y = r.y + ( r.height ) / 2;
	drawEllipse( x, y, 2, 2 );
	drawRectangle( r.x, r.y, r.width, r.height );
	dc->DrawRotatedText( s, x, y, angle );
}

/*****************************************************
**
**   DcHelper   ---   drawTextFormatted
**
******************************************************/
void DcHelper::drawTextFormatted( const wxRect &rect, const wxString t, const int& align )
{
  wxCoord w, h;
  dc->GetTextExtent( t, &w, &h );
  int xmitte, ymitte, x, y;

  ymitte = 2 * rect.y + rect.height;
  ymitte /= 2;
  if ( align & Align::VCenter ) y = ymitte - h /2;
  else if ( align & Align::Top ) y = rect.y;
  else y = rect.y + rect.height - h;

  xmitte = 2 * rect.x + rect.width;
  xmitte /= 2;
  if ( align & Align::HCenter ) x = xmitte - w /2;
  else if ( align & Align::Left ) x = rect.x;
  else x = rect.x + rect.width - w;
  dc->DrawText( t, x, y );
}


