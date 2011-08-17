/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfPainter.h
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

#ifndef _PDFPAINTER_H_
#define _PDFPAINTER_H_

#ifdef __GNUG__
	#pragma interface "PdfPainter.h"
#endif

#include "Painter.h"

#include <wx/string.h>

class BasePdfDocument;
class Writer;

class wxPdfLineStyle;

/*************************************************//**
*
* \brief painter implementation for PDF printout
*
******************************************************/
class PdfPainter : public Painter
{
public:
	PdfPainter( BasePdfDocument* );

	virtual ~PdfPainter();

	// paint graphical items
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 );
	virtual void drawRectangle( const wxRect& );

	virtual void drawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 );
  virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 );
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax );
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
		const double &w1, const double &w2 );
	virtual void drawPoint( const double &x, const double &y );

	// Text output
	virtual void drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
		const wxString &t, const int& align );
	virtual void getTextExtent( const wxString &s, double *w, double *h );
	virtual void drawSimpleRotatedText( const wxRect &r, const wxString &s, const double &angle );
	virtual void drawSignSymbol( const int&x, const int &y, const int &sign, const int type = 0, const int zoom = 100, const double angle = 0 );

	// Brush
	virtual void setTransparentBrush();
	virtual void setBrushColor( const wxColour& );
	virtual wxColour getBrushColor();
	virtual void setBackgroundImage( const wxBitmap& ) {}
	virtual void setBackgroundImage( const wxString& ) {}

	// Pen
	virtual void setTransparentPen();
	virtual void setPenColor( const wxColour& );
	virtual void setPen( const wxColour &color, const int &width, const int &linestyle );
	virtual wxColour getPenColor();
	virtual void setLineStyle( const int& );

	// fonts and text properties
	void setSymbolFont( const int = 100 );
	void setGraphicFont( const int = 100 );
	virtual void setFontSize( const int& ) {}
	virtual void setTextBackgroundColor( const wxColour& ) {}
	virtual void setTextColor( const wxColour& );

	virtual void doFloodFill( const wxString &bgImage, const wxColour &fgcolor, const wxColour &bgcolor, const wxRect &rect );

protected:

	wxPdfLineStyle *lsSolid, *lsDot, *lsLongDash, *lsDotDash;
	Writer *textwriter;

	int drawmode;

	wxString symbolFontConfigName;

private:
	BasePdfDocument *pdf;
};

#endif


