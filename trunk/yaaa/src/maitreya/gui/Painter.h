/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/Painter.h
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

#ifndef _PAINTER_H_
#define _PAINTER_H_

#ifdef __GNUG__
	#pragma interface "Painter.h"
#endif

#include "guibase.h"
#include <wx/string.h>

class DasaColorConfig;
class wxBitmap;
class wxColour;
class wxDC;

/*************************************************//**
*
* \brief abstract class for paiting graphical items and text on a GUI
*
******************************************************/
class Painter
{
public:

	virtual ~Painter() {}

	// ************ graphical items section ****************
	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 ) = 0;

	/**
	*  \brief delegates
	*/
	void drawRectangle( const wxRect&, const double = 0 );

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 ) = 0;

	/**
	*  \brief abstract, must be overwritten
	*/
  virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 ) = 0;

	/**
	*  \brief delegates
	*/
	void drawLine( const wxPoint &p1, const wxPoint &p2 );

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax ) = 0;

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
		const double &w1, const double &w2 ) = 0;

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawPoint( const double &x, const double &y ) = 0;

	// ************ text output section ****************
	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
		const wxString &t, const int& align ) = 0;

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void getTextExtent( const wxString &s, double *w, double *h ) = 0;

	/**
	*  \brief delegate
	*/
	void drawSimpleText( const wxRect &r, const wxString &s );

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawSimpleRotatedText( const wxRect &r, const wxString &s, const double &angle ) = 0;

	/**
	*  \brief delegate
	*/
	virtual void drawSimpleText( const double &x, const double &y, const double &w, const double &h,
		const wxString &s );

	/**
	*  \brief delegate
	*/
	void drawTextFormatted( const wxRect &rect, const wxString &t, const int& align );

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void drawSignSymbol( const int&x, const int &y, const int &sign, const int type = 0, const int zoom = 100, const double angle = 0 ) = 0;


	// ************ pen section ****************
	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void setTransparentPen() = 0;

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void setPenColor( const wxColour& ) = 0;

	/**
	*  \brief implement in subclass
	*/
	virtual void setPenWidth( const int& ) {}

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void setLineStyle( const int& ) = 0;

	/**
	*  \brief delegate
	*/
	virtual void setPen( const wxPen& );

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void setPen( const wxColour &color, const int &width, const int &linestyle ) = 0;

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual wxColour getPenColor() = 0;

	// ************ brush section ****************
	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void setTransparentBrush() = 0;

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual void setBrushColor( const wxColour& ) = 0;

	/**
	*  \brief delegate
	*/
	virtual void setBrush( const wxBrush& );

	/**
	*  \brief abstract, must be overwritten
	*/
	virtual wxColour getBrushColor() = 0;

	virtual void setBackgroundImage( const wxBitmap& ) = 0;
	virtual void setBackgroundImage( const wxString& ) = 0;
	virtual bool supportsBackgroundImages() { return true; }
	
	virtual void setTextColor( const wxColour& ) = 0;
	virtual void setTextBackgroundColor( const wxColour& ) = 0;
	virtual void setSymbolFont( const int = 100 ) = 0;
	virtual void setGraphicFont( const int = 100 ) = 0;
	virtual void setFontSize( const int& ) = 0;

	virtual void doFloodFill( const wxString &bgImage, const wxColour &fgcolor, const wxColour &bgcolor, const wxRect &rect );


	// ************ graphical Dasa section ****************
	void setDasaColors( const int &object, const int &rasitype );
	void setDasaColors( const int &objectid );
	void drawDasaBar( const int &dasalord, const int &isRasiLord, const DasaColorConfig *cfg, const int &x1, const int &y1,
		const int &w, const int &h, const int &cornerRadius );

};

/*************************************************//**
*
* \brief Painter implementation for wx device context (GUI and file export via memory DC)
*
******************************************************/
class DcPainter: public Painter
{
public:
	DcPainter( wxDC* );
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 );
	virtual void drawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 );
  virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 );
	virtual void drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
		const wxString &t, const int& align );
	virtual void getTextExtent( const wxString &s, double *w, double *h );
	virtual void drawSimpleRotatedText( const wxRect &r, const wxString &s, const double &angle );
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax );
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
		const double &w1, const double &w2 );
	virtual void drawPoint( const double &x, const double &y );

	// Brush
	virtual void setTransparentBrush();
	virtual void setBrushColor( const wxColour& );
	virtual void setBrush( const wxBrush& );
	virtual wxColour getBrushColor();

	// Pen
	virtual void setTransparentPen();
	virtual void setPenColor( const wxColour& );
	virtual void setPenWidth( const int& );
	virtual void setPen( const wxColour &color, const int &width, const int &linestyle );
	virtual wxColour getPenColor();
	virtual void setLineStyle( const int& );

	virtual void setBackgroundImage( const wxBitmap& );
	virtual void setBackgroundImage( const wxString& );
	virtual void setSymbolFont( const int = 100 );
	virtual void setGraphicFont( const int = 100 );
	virtual void setTextColor( const wxColour& );
	virtual void setTextBackgroundColor( const wxColour& );
	virtual void setFontSize( const int& );
	virtual void drawSignSymbol( const int&x, const int &y, const int &sign, const int type = 0, const int zoom = 100, const double angle = 0 );

protected:

private:
	wxDC *dc;
};

/*************************************************//**
*
* \brief helper class for drawing text
*
******************************************************/
class DcHelper
{
public:
	DcHelper( wxDC *d ) { dc = d; }
	void drawTextFormatted( const wxRect &rect, const wxString t, const int& align );

protected:
	wxDC* dc;

private:
	DcHelper() {} // not available
};


#endif

