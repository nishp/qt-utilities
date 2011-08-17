/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfWriter.cpp
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
  #pragma implementation "PdfWriter.h"
#endif

#include "PdfWriter.h"

#include "Conf.h"
//#include "config.h"
#include "Lang.h"
#include "LanguageConfig.h"
#include "PdfBase.h"

#ifdef __WXMSW__
#include <wx/mimetype.h>
#endif

#include <wx/utils.h>

extern Config *config;

/*****************************************************
**
**   PdfWriter   ---   Constructor
**
******************************************************/
PdfWriter::PdfWriter( BasePdfDocument *pdfdoc, Config *c )
 : Writer( c )
{
  pdf = pdfdoc;
  type = WRITER_PDF;
}

/*****************************************************
**
**   PdfWriter   ---   Destructor
**
******************************************************/
PdfWriter::~PdfWriter()
{
  // pdfdoc instance belongs to PdfPrinter class.
}

/*****************************************************
**
**   PdfWriter   ---   beginWriting
**
******************************************************/
void PdfWriter::beginWriting()
{
  wxString s;
  pdf->AliasNbPages();

  pdf->SetAuthor( wxGetUserName() );
  s << GUI_APP_NAME << wxT( " " ) << wxConvertMB2WX( VERSION );
  pdf->SetCreator( s );
}

/*****************************************************
**
**   PdfWriter   ---   endWriting
**
******************************************************/
void PdfWriter::endWriting()
{
  pdf->SaveAsFile( filename );
}

/*****************************************************
**
**   PdfWriter   ---   getSignName
**
******************************************************/
const wxString PdfWriter::getSignName( const int &i, const int format )

{
  if ( ! cfg->useSignSymbols ) return Writer::getSignName( i, format );
  return lang->getSignSymbolCode( i );
}

/*****************************************************
**
**   PdfWriter   ---   getRetroSymbol
**
******************************************************/
const wxString PdfWriter::getRetroSymbol(  const int type )
{
  if ( ! config->useSignSymbols ) return Writer::getRetroSymbol();
  return( type ? wxT( "^" ) : wxT( "_" ) );
}

/*****************************************************
**
**   PdfWriter   ---   getObjectName
**
******************************************************/
const wxString PdfWriter::getObjectName( const int &num, const int &format, const bool vedic )
{
 if ( num >= OFORTUNE || ! cfg->usePlanetSymbols || ( num >= ODHUMA && num <= LAST_EXTENDED_OBJECT ))
   return Writer::getObjectName( num, format, vedic );
 else
  return lang->getPlanetSymbolCode( num );
}

/*****************************************************
**
**   PdfWriter   ---   writeHeader1
**
******************************************************/
void PdfWriter::writeHeader1( const wxString &s1 )
{
  pdf->setHeader1Font();
  pdf->MultiCell( 0, 7, s1, wxPDF_BORDER_NONE, wxPDF_ALIGN_CENTER );
  pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---   writeHeader2
**
******************************************************/
void PdfWriter::writeHeader2( const wxString &s1 )
{
  pdf->setHeader2Font();
  pdf->MultiCell(0,5,s1);
  pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---   writeHeader3
**
******************************************************/
void PdfWriter::writeHeader3( const wxString &s1 )
{
  pdf->setHeader3Font();
  pdf->MultiCell(0,4,s1);
  pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---   writeParagraph
**
******************************************************/
void PdfWriter::writeParagraph( const wxString &s1 )
{
  pdf->MultiCell(0,4,s1);
}

/*****************************************************
**
**   PdfWriter   ---   writeLine
**
******************************************************/
void PdfWriter::writeLine( const wxString &s1 )
{
  pdf->MultiCell(0,4,s1);
}

/*****************************************************
**
**   PdfWriter   ---   writeTableHeaderEntry
**
******************************************************/
void PdfWriter::writeTableHeaderEntry( const wxString &s1 )
{
}

/*****************************************************
**
**   PdfWriter   ---   writeTable
**
******************************************************/
void PdfWriter::writeTable( Table &t )
{
  int c, r;
  double d;
  TableEntry entry;
  Row row;
  int align, frame;
  double total_width = 0, left_p;

  pdf->setDefaultFont();

  for( r = 0; r < (int)t.nb_rows; r++ )
  {
    row = t.contents[r];
    for( c = 0; c < (int)t.nb_cols; c++ )
    {
      entry = row.value[c];
      d = pdf->GetStringWidth( entry.value );
      t.contents[r].value[c].width = (int)d+3;
      t.col_width[c] = Max( (int)d+3, t.col_width[c] );
    }
  }
  for ( c = 0; c < (int)t.nb_cols; c++ )
  {
    total_width += t.col_width[c];
  }
  double pw = pdf->GetPageWidth() - pdf->GetLeftMargin() - pdf->GetRightMargin();

  // bugfix: left_p will be negative if width to high, output will be broken on right side
  left_p = pdf->GetLeftMargin();
  if ( total_width <= pdf->GetPageWidth() )
  {
    left_p = pdf->GetLeftMargin() + ( pw - total_width ) / 2;
  }
  pdf->SetX( left_p );
  for( r = 0; r < (int)t.nb_rows; r++ )
  {
    row = t.contents[r];
    for( c = 0; c < (int)t.nb_cols; c++ )
    {
      frame = wxPDF_BORDER_NONE;
      if ( c == 0 ) frame = wxPDF_BORDER_LEFT;
      if ( c == (int)t.nb_cols-1 ) frame = wxPDF_BORDER_RIGHT;
      if ( r == 0 || r == 1 ) frame |= wxPDF_BORDER_TOP;
      if ( r == (int)t.nb_rows-1 ) frame |= wxPDF_BORDER_BOTTOM;
      if ( t.col_line[c] ) frame |= wxPDF_BORDER_RIGHT;

      entry = row.value[c];
      if ( entry.isSymbol )
      {
        pdf->setSymbolFont();
      }
      else if ( entry.isHeader ) pdf->setDefaultBoldFont();
      else pdf->setDefaultFont();

      if ( t.col_alignment[c] & Align::Left ) align = wxPDF_ALIGN_LEFT;
      else if ( t.col_alignment[c] & Align::Right ) align = wxPDF_ALIGN_RIGHT;
      else align = wxPDF_ALIGN_CENTER;
      pdf->Cell( t.col_width[c], 5, entry.value, frame, 0, align );
    }
    pdf->Ln();
    pdf->SetX( left_p );
  }
  pdf->SetX( pdf->GetLeftMargin() );
  pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---  writeListInColumns
**
******************************************************/
void PdfWriter::writeListInColumns( const vector<wxString> v, const int &colnr )
{
  unsigned int i, j, index;
  unsigned int ePerCol = (int)v.size() / colnr;
  if ( (int)v.size() % colnr ) ePerCol++;

  Table table( colnr, ePerCol );
  for( i = 0; i < (unsigned int)colnr; i++ )
  {
    table.col_alignment[i] = Align::Left;
    for( j = 0; j < ePerCol; j++ )
    {
      index = i*ePerCol + j;
      if ( index < v.size() ) table.setEntry( i, j, v[index] );
    }
    s << Endl;
  }
  writeTable( table );
}

/*****************************************************
**
**   PdfWriter   ---  writeUranianDiagram
**
******************************************************/
void PdfWriter::writeUranianDiagram( vector<UranianDiagram> mp )
{
  // not yet implemented
}
