/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfPrinter.h
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

#ifndef PDFPRINTER_H_
#define PDFPRINTER_H_

#ifdef __GNUG__
	#pragma interface "PdfPrinter.h"
#endif

#include <wx/string.h>

class BasePdfDocument;
class ChartProperties;
class Horoscope;
class Painter;
class PrintoutConfig;
class PrintoutConfigItem;
class PdfWriter;

/*************************************************//**
*
* \brief main class for creation of PDF printout
*
******************************************************/
class PdfPrinter
{
public:

	PdfPrinter( const Horoscope* );
	~PdfPrinter();

	bool print( const int &prt, wxString, const bool askOverwrite );
	
protected:

	bool preparePrint( wxString filename, const wxString basename, const bool askOverwrite );

	void printVedicPair( const int &varga1, const int &varga2, int style = -1 );
	void printWesternChart( int style = -1 );
	void printItem( const PrintoutConfigItem* );
	void printSbc();
	void printTitle();
	void printUranian();
	void launchBrowser();

  const Horoscope *h;

	BasePdfDocument *pdfdoc;
	Painter *painter;
	PdfWriter *writer;
	ChartProperties *props;

	wxString filename;
};


#endif

