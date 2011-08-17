/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfWriter.h
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

#ifndef _PDFWRITER_H_
#define _PDFWRITER_H_

#ifdef __GNUG__
	#pragma interface "PdfWriter.h"
#endif

#include "Writer.h"

#include <wx/gdicmn.h>
#include <wx/string.h>

class BasePdfDocument;

/*************************************************//**
*
* \brief Writer for PDF printout
*
******************************************************/
class PdfWriter : public Writer
{
public:
	PdfWriter( BasePdfDocument*, Config* );

	virtual ~PdfWriter();

	// Writer
	virtual void beginWriting();
	virtual void endWriting();

	virtual void writeHeader1( const wxString& );
	virtual void writeHeader2( const wxString& );
	virtual void writeHeader3( const wxString& );
	virtual void writeParagraph( const wxString& );
	virtual void writeLine( const wxString& );
	virtual void writeTable( Table& );
	virtual void writeListInColumns( const vector<wxString>, const int &colnr );
	virtual void writeUranianDiagram( vector<UranianDiagram> );
	virtual const wxString getSignName( const int &i, const int format = TMEDIUM );
	virtual const wxString getRetroSymbol(  const int type = 0 );

	virtual const wxString getObjectName( const int &num, const int &format, const bool vedic );

	void setFilename( wxString f ) { filename = f; }
	
protected:

	wxString filename;

	class BasePdfDocument *pdf;
		
	void writeTableHeaderEntry( const wxString &s1 );
};

#endif


