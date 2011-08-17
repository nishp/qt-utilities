/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfPainter.cpp
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
	#pragma implementation "PdfPainter.h"
#endif

#include "PdfPainter.h"
#include "PdfBase.h"

#include "Conf.h"
#include "Lang.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   PdfPainter   ---   Constructor
**
******************************************************/
PdfPainter::PdfPainter( BasePdfDocument *pdfdoc )
{
	pdf = pdfdoc;
	drawmode = wxPDF_STYLE_DRAW;
	textwriter = WriterFactory().getWriter( WRITER_TEXT );

	// line style solid
	lsSolid = new wxPdfLineStyle();
  wxPdfArrayDouble dash1;
  dash1.Add(10.);
  dash1.Add(20.);
  dash1.Add(5.);
  dash1.Add(10.);

	// line style dot
  lsDot = new wxPdfLineStyle( -1, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash1, 10.);

	// line style long dash
  lsLongDash = new wxPdfLineStyle( -1, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash1, 0.);

	// line style dot dash
  wxPdfArrayDouble dash3;
  dash3.Add(2.);
  dash3.Add(10.);
  lsDotDash = new wxPdfLineStyle( -1, wxPDF_LINECAP_ROUND, wxPDF_LINEJOIN_ROUND, dash3, 0.);
}

/*****************************************************
**
**   PdfPainter   ---   Destructor
**
******************************************************/
PdfPainter::~PdfPainter()
{
	delete lsSolid;
	delete lsDot;
	delete lsLongDash;
	delete lsDotDash;
	delete textwriter;
}

/*****************************************************
**
**   PdfPainter   ---   setGraphicFont
**  
******************************************************/ 
void PdfPainter::setGraphicFont( const int zoom )
{ 
	pdf->setDefaultFont();
} 

/*****************************************************
**
**   PdfPainter   ---   setSymbolFont
**  
******************************************************/ 
void PdfPainter::setSymbolFont( const int zoom )
{ 
	pdf->setSymbolFont( zoom );
} 

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const double &x, const double &y, const double &w, const double &h, const double rnd )
{
	// TODO round not implemented
	pdf->Rect( x, y, w, h, drawmode );
}

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const wxRect &rect )
{
	drawRectangle( rect.x, rect.y, rect.width, rect.height );
}

/*****************************************************
**
**   PdfPainter   ---   drawPolygon
**
******************************************************/
void PdfPainter::drawPolygon(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	wxPdfArrayDouble x, y;
	for( int i = 0; i < n; i++ )
	{
		x.Add( points[i].x + xoffset );
		y.Add( points[i].y + yoffset );
	}
	pdf->Polygon( x, y, drawmode );
}

/*****************************************************
**
**   PdfPainter   ---  drawLine
**
******************************************************/
void PdfPainter::drawLine( const double &x1, const double &y1, const double &x2, const double &y2 )
{
	pdf->Line( x1, y1, x2, y2 );
}

/*****************************************************
**
**   PdfPainter   ---  drawTextFormatted
**
******************************************************/
void PdfPainter::drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
	const wxString &t, const int& align )
{
	double sw, sh;
	double oldx = pdf->GetX();
	double oldy = pdf->GetY();
	double x1 = x;
	//double w1 = w;
	double y1 = y;
	double h1 = h;

	pdf->SetXY( x, y1 );

	getTextExtent( t, &sw, &sh );

	if ( align & Align::Top )
	{
		h1 = sh;

		// bugfix 6.0dev3: Arudhas overwrite planet symbols
		y1 = y + h; // + .5 * sh;
	}
	else if ( align & Align::Bottom )
	{
		y1 = y + h; // + .5 * sh;
		h1 = sh;
	}
	else if ( align & Align::VCenter )
	{
		y1 = y + .5 * ( h + sh );
	}
	if ( align & Align::HCenter )
	{
		x1 = x + .5 * ( w - sw );
	}
	else if ( align & Align::Right )
	{
		x1 = x + w - sw;
	}
	pdf->Text( x1, y1, t );
	//drawEllipse( x1, y1, 2, 2 );
	pdf->SetXY( oldx, oldy );
}

/*****************************************************
**
**   PdfPainter   ---  getTextExtent
**
******************************************************/
void PdfPainter::getTextExtent( const wxString &s, double *w, double *h )
{
	const int defaultFontSize = 10;
	*w = pdf->GetStringWidth( s );
	if ( s.Len() == 1 ) *w *= 1.7; // make single symbol items larger
	*h = .5 * defaultFontSize;
}

/*****************************************************
**
**   PdfPainter   ---  setTextColor
**
******************************************************/
void PdfPainter::setTextColor( const wxColour &c )
{
	if ( config->printUseColors ) pdf->SetTextColour( c );
}

/*****************************************************
**
**   PdfPainter   ---   drawEllipse
**
******************************************************/
void PdfPainter::drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2 ); // Cannot be filled
}

/*****************************************************
**
**   PdfPainter   ---   drawSimpleRotatedText
**
******************************************************/
void PdfPainter::drawSimpleRotatedText( const wxRect &r, const wxString &s, const double &angle )
{
	// TODO
}

/*****************************************************
**
**   PdfPainter   ---   drawArc 
**
******************************************************/
void PdfPainter::drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
	const double &w1, const double &w2 )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2, 0, w1, w2 );
}

/*****************************************************
**
**   PdfPainter   ---   drawPoint 
**
******************************************************/
void PdfPainter::drawPoint( const double &x, const double &y )
{
	pdf->Line( x, y, x, y );
}

/*****************************************************
**
**   PdfPainter   ---   setTransparentBrush
**
******************************************************/
void PdfPainter::setTransparentBrush()
{
	drawmode = wxPDF_STYLE_DRAW;
}

/*****************************************************
**
**   PdfPainter   ---   setBrushColor
**
******************************************************/
void PdfPainter::setBrushColor( const wxColour &c )
{
	drawmode = wxPDF_STYLE_FILL;
	if ( config->printUseColors ) pdf->SetFillColour( c );
	else pdf->SetFillColour( *wxWHITE );
}

/*****************************************************
**
**   PdfPainter   ---   getBrushColor
**
******************************************************/
wxColour PdfPainter::getBrushColor()
{
	return *wxWHITE;
}

/*****************************************************
**
**   PdfPainter   ---   setTransparentPen
**
******************************************************/
void PdfPainter::setTransparentPen()
{
	pdf->SetDrawColour( *wxWHITE );
}

/*****************************************************
**
**   PdfPainter   ---   setPen
**
******************************************************/
void PdfPainter::setPen( const wxColour &color, const int &width, const int &linestyle )
{
	setPenColor( color );
	
	// not implemented, only in Painter base class
	//setPenWidth( width );

	// BUG? line is black without style if called
	//setLineStyle( linestyle );
}

/*****************************************************
**
**   PdfPainter   ---   setPenColor
**
******************************************************/
void PdfPainter::setPenColor( const wxColour &c )
{
	if ( config->printUseColors ) pdf->SetDrawColour( c );
}

/*****************************************************
**
**   PdfPainter   ---   getPenColor
**
******************************************************/
wxColour PdfPainter::getPenColor()
{
	return *wxBLACK;
}

/*****************************************************
**
**   PdfPainter   ---   setLineStyle
**
******************************************************/
void PdfPainter::setLineStyle( const int &s )
{
	switch( s )
	{
		case wxDOT_DASH:
			pdf->SetLineStyle( *lsDotDash );
		break;
		case wxDOT:
			pdf->SetLineStyle( *lsDot );
		break;
		case wxLONG_DASH:
			pdf->SetLineStyle( *lsLongDash );
		break;
		default:
			pdf->SetLineStyle( *lsSolid );
		break;
	}
}

/*****************************************************
**
**   PdfPainter   ---   drawSignSymbol
**
******************************************************/
void PdfPainter::drawSignSymbol( const int&x, const int &y, const int &sign, const int type, const int zoom, const double angle )
{
	Lang lang;
	double w, h;
	wxString s;

	if ( type == 2 ) s = textwriter->getSignName( sign, TSHORT );
	else s = lang.getSignSymbolCode( sign, type );
	getTextExtent( s, &w, &h );

	// TODO why
	h *= 2.5;

	double x4 = x - .5 * w * cos( .25 * PI + angle * DEG2RAD );
	double y4 = y + .5 * h * sin( .25 * PI + angle * DEG2RAD );

	pdf->RotatedText( x4, y4, s, angle );
}

/*****************************************************
**
**   PdfPainter   ---   doFloodFill
**
******************************************************/
void PdfPainter::doFloodFill( const wxString &bgImage, const wxColour &fgColor, const wxColour &bgColor, const wxRect &rect )
{
	if ( ! bgImage.IsEmpty() )
	{
		tellThatPdfSupportsNoBgImages( 0 );
	}
	setBrushColor( bgColor );
	setPenColor( bgColor );
	drawRectangle( rect );
	setTransparentBrush();
	setPenColor( fgColor );
}

